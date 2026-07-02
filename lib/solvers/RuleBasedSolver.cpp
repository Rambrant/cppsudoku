//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "RuleBasedSolver.hpp"

#include <algorithm>

#include "core/Logger.hpp"
#include "core/SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    namespace detail
    {
        using Square = std::pair<int, int>;
        using Unit   = std::array<Square, Traits::BOARD_SIZE>;  // A row, column, or box — 9 cells.

        static auto hasCandidate( const CandidateSet& candidates, Traits::Value val) -> bool
        {
            return std::find( candidates.begin(), candidates.end(), val) != candidates.end();
        }

        static auto removeFrom( CandidateSet& candidates, Traits::Value value) -> bool
        {
            auto it = std::find( candidates.begin(), candidates.end(), value);

            if( it == candidates.end())
            {
                return false;
            }

            candidates.erase( it);

            return true;
        }

        static auto makeRowUnit( int row) -> Unit
        {
            Unit unit{};

            for( int colIdx : Traits::INDEX_RANGE)
            {
                unit[ colIdx] = { row, colIdx};
            }

            return unit;
        }

        static auto makeColUnit( int col) -> Unit
        {
            Unit unit{};

            for( int rowIdx : Traits::INDEX_RANGE)
            {
                unit[ rowIdx] = { rowIdx, col};
            }

            return unit;
        }

        /**
         * @param boxRow  Top-left row of the box (a multiple of @ref SudokuTraits::BOX_SIZE).
         * @param boxCol  Top-left column of the box.
         */
        static auto makeBoxUnit( int boxRow, int boxCol) -> Unit
        {
            Unit  unit{};
            int   unitIdx = 0;

            for( int rowIdx = boxRow; rowIdx < boxRow + Traits::BOX_SIZE; ++rowIdx)
            {
                for( int colIdx = boxCol; colIdx < boxCol + Traits::BOX_SIZE; ++colIdx)
                {
                    unit[ unitIdx++] = { rowIdx, colIdx};
                }
            }

            return unit;
        }

        /**
         *  @brief Calls @p func(unit) for all 27 units (9 rows + 9 cols + 9 boxes).
         */
        template<typename Func>
        static void forAllUnits( Func func)
        {
            for( int rowIdx : Traits::INDEX_RANGE)
            {
                func( makeRowUnit( rowIdx));
            }

            for( int colIdx : Traits::INDEX_RANGE)
            {
                func( makeColUnit( colIdx));
            }

            for( int boxRowIdx = 0; boxRowIdx < Traits::BOARD_SIZE; boxRowIdx += Traits::BOX_SIZE)
            {
                for( int boxColIdx = 0; boxColIdx < Traits::BOARD_SIZE; boxColIdx += Traits::BOX_SIZE)
                {
                    func( makeBoxUnit( boxRowIdx, boxColIdx));
                }
            }
        }

        /**
         * @brief Calls @p func(peerRow, peerCol) for every peer of @p (row, col) —
         *        all cells in the same row, column, or box, excluding the cell itself.
         */
        template<typename Func>
        static void forAllPeers( int row, int col, Func func)
        {
            for( int colIdx : Traits::INDEX_RANGE)
            {
                if( colIdx != col)
                {
                    func( row, colIdx);
                }
            }

            for( int colIdx : Traits::INDEX_RANGE)
            {
                if( colIdx != row)
                {
                    func( colIdx, col);
                }
            }

            const int startRow = (row / Traits::BOX_SIZE) * Traits::BOX_SIZE;
            const int startCol = (col / Traits::BOX_SIZE) * Traits::BOX_SIZE;

            for( int rowIdx = startRow; rowIdx < startRow + Traits::BOX_SIZE; ++rowIdx)
            {
                for( int colIdx = startCol; colIdx < startCol + Traits::BOX_SIZE; ++colIdx)
                {
                    if( rowIdx != row || colIdx != col)
                    {
                        func( rowIdx, colIdx);
                    }
                }
            }
        }

        auto initCandidates( const Traits::Board& board) -> std::expected<CandidateGrid, std::string>
        {
            CandidateGrid grid{};

            //
            // All cells start with every value as a candidate.
            //
            for( int rowIdx : Traits::INDEX_RANGE)
            {
                for( int colIdx : Traits::INDEX_RANGE)
                {
                    grid[rowIdx][colIdx] = std::vector<Traits::Value>( Traits::VALUE_RANGE.begin(),
                                                             Traits::VALUE_RANGE.end());
                }
            }

            //
            // Fix each given cell and remove its value from peers.
            //
            for( int rowIdx : Traits::INDEX_RANGE)
            {
                for( int colIdx : Traits::INDEX_RANGE)
                {
                    const auto val = board[rowIdx][colIdx];

                    {
                        if( val == Traits::NO_VALUE) continue;
                    }

                    //
                    // If a prior peer already eliminated this value, it's a contradiction.
                    //
                    if( ! hasCandidate( grid[rowIdx][colIdx], val))
                    {
                        return std::unexpected{
                            "Illegal board: contradiction at [" +
                            std::to_string( rowIdx) + "," + std::to_string( colIdx) + "]"
                        };
                    }

                    grid[rowIdx][colIdx] = { val };

                    forAllPeers( rowIdx, colIdx, [&]( int peerRow, int peerCol)
                    {
                        removeFrom( grid[peerRow][peerCol], val);
                    });
                }
            }

            return grid;
        }

        auto isSolved( const CandidateGrid& grid) -> bool
        {
            for( int rowIdx : Traits::INDEX_RANGE)
            {
                for( int colIdx : Traits::INDEX_RANGE)
                {
                    if( grid[rowIdx][colIdx].size() != 1)
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        auto isContradicted( const CandidateGrid& grid) -> bool
        {
            for( int rowIdx : Traits::INDEX_RANGE)
            {
                for( int colIdx : Traits::INDEX_RANGE)
                {
                    if( grid[rowIdx][colIdx].empty())
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        auto toBoard( const CandidateGrid& grid) -> Traits::Board
        {
            Traits::Board board{};

            for( int rowIdx : Traits::INDEX_RANGE)
            {
                for( int colIdx : Traits::INDEX_RANGE)
                {
                    board[rowIdx][colIdx] = (grid[rowIdx][colIdx].size() == 1) ? grid[rowIdx][colIdx][0] : Traits::NO_VALUE;
                }
            }

            return board;
        }

        //
        // Rule 1: Naked Single
        //
        // A cell with exactly one candidate: eliminate that value from all unit peers.
        //
        auto applyNakedSingles( CandidateGrid& grid) -> bool
        {
            bool progress = false;

            for( int rowIdx : Traits::INDEX_RANGE)
            {
                for( int colIdx : Traits::INDEX_RANGE)
                {
                    if( grid[rowIdx][colIdx].size() != 1)
                    {
                        continue;
                    }

                    const Traits::Value val = grid[rowIdx][colIdx][0];

                    forAllPeers( rowIdx, colIdx, [&]( int peerRow, int peerCol)
                    {
                        progress |= removeFrom( grid[peerRow][peerCol], val);
                    });
                }
            }

            return progress;
        }

        //
        // Rule 2: Hidden Single
        //
        // Within a unit, a value that can only go in one cell → assign it there.
        //
        static auto hiddenSinglesInUnit( CandidateGrid& grid, const Unit& unit) -> bool
        {
            bool progress = false;

            for( const Traits::Value val : Traits::VALUE_RANGE)
            {
                int rowCount = 0;
                int lastRow  = -1;
                int lastCol  = -1;

                for( auto [row, col] : unit)
                {
                    if( hasCandidate( grid[row][col], val))
                    {
                        ++rowCount;
                        lastRow = row;
                        lastCol = col;
                    }
                }

                if( rowCount == 1 && grid[lastRow][lastCol].size() > 1)
                {
                    grid[lastRow][lastCol] = { val };
                    progress = true;
                }
            }

            return progress;
        }

        auto applyHiddenSingles( CandidateGrid& grid) -> bool
        {
            bool progress = false;

            forAllUnits( [&]( const Unit& unit)
            {
                progress |= hiddenSinglesInUnit( grid, unit);
            });

            return progress;
        }

        //
        // Rule 3: Naked Pair
        //
        // Two cells in a unit whose union of candidates is exactly two values.
        // Eliminate those two values from all other cells in the unit.
        //
        static auto nakedPairsInUnit( CandidateGrid& grid, const Unit& unit) -> bool
        {
            bool progress = false;

            for( size_t unitIdx = 0; unitIdx < unit.size(); ++unitIdx)
            {
                auto [r1, c1] = unit[unitIdx];
                const auto& s1 = grid[r1][c1];

                if( s1.empty() || s1.size() > 2)
                {
                    continue;
                }

                for( size_t j = unitIdx + 1; j < unit.size(); ++j)
                {
                    auto [r2, c2] = unit[j];
                    const auto& s2 = grid[r2][c2];

                    if( s2.empty() || s2.size() > 2)
                    {
                        continue;
                    }

                    //
                    // Union of s1 and s2 — must equal exactly 2 values to form a naked pair.
                    //
                    std::vector<Traits::Value> unionValues( s1.begin(), s1.end());
                    for( Traits::Value value : s2)
                    {
                        if( ! hasCandidate( unionValues, value))
                        {
                            unionValues.push_back( value);
                        }
                    }

                    if( unionValues.size() != 2)
                    {
                        continue;
                    }

                    //
                    // Found a naked pair: eliminate both values from all other cells in the unit.
                    //
                    for( auto [row, col] : unit)
                    {
                        if( (row == r1 && col == c1) || (row == r2 && col == c2))
                        {
                            continue;
                        }

                        for( Traits::Value value : unionValues)
                        {
                            progress |= removeFrom( grid[row][col], value);
                        }
                    }
                }
            }

            return progress;
        }

        auto applyNakedPairs( CandidateGrid& grid) -> bool
        {
            bool progress = false;

            forAllUnits( [&]( const Unit& unit)
            {
                progress |= nakedPairsInUnit( grid, unit);
            });

            return progress;
        }

        //
        // Rule 4: Hidden Pair
        //
        // Two values that appear in exactly the same two cells of a unit.
        // Remove all other candidates from those two cells.
        //
        static auto hiddenPairsInUnit( CandidateGrid& grid, const Unit& unit) -> bool
        {
            bool progress = false;

            for( int v1 = 1; v1 <= Traits::MAX_VALUE; ++v1)
            {
                for( int v2 = v1 + 1; v2 <= Traits::MAX_VALUE; ++v2)
                {
                    std::vector<Square> withV1;
                    std::vector<Square> withV2;

                    for( auto [row, col] : unit)
                    {
                        if( hasCandidate( grid[row][col], v1)) withV1.push_back( {row, col});
                        if( hasCandidate( grid[row][col], v2)) withV2.push_back( {row, col});
                    }

                    //
                    // Hidden pair: v1 and v2 both appear in exactly the same two cells.
                    //
                    if( withV1.size() != 2 || withV2.size() != 2)
                    {
                        continue;
                    }

                    if( withV1 != withV2)
                    {
                        continue;
                    }

                    for( auto [row, col] : withV1)
                    {
                        auto& candidates = grid[row][col];

                        if( candidates.size() <= 2)
                        {
                            continue;    // already a naked pair or single
                        }

                        std::vector<Traits::Value> restricted;

                        for( Traits::Value value : candidates)
                        {
                            if( value == v1 || value == v2) restricted.push_back( value);
                        }

                        if( restricted.size() < candidates.size())
                        {
                            candidates = std::move( restricted);
                            progress = true;
                        }
                    }
                }
            }

            return progress;
        }

        auto applyHiddenPairs( CandidateGrid& grid) -> bool
        {
            bool progress = false;

            forAllUnits( [&]( const Unit& unit)
            {
                progress |= hiddenPairsInUnit( grid, unit);
            });

            return progress;
        }

        //
        // Rule 5: Pointing Pair / Triple
        //
        // When all occurrences of a value inside a box lie in the same row or column,
        // eliminate that value from the rest of that row or column (outside the box).
        //
        auto applyPointingPairs( CandidateGrid& grid) -> bool
        {
            bool progress = false;

            for( int boxRow = 0; boxRow < Traits::BOARD_SIZE; boxRow += Traits::BOX_SIZE)
            {
                for( int boxCol = 0; boxCol < Traits::BOARD_SIZE; boxCol += Traits::BOX_SIZE)
                {
                    for( const Traits::Value val : Traits::VALUE_RANGE)
                    {
                        //
                        // Collect cells inside this box that carry val as a candidate.
                        //
                        std::vector<Square> cells;
                        for( int rowIdx = boxRow; rowIdx < boxRow + Traits::BOX_SIZE; ++rowIdx)
                        {
                            for( int colIdx = boxCol; colIdx < boxCol + Traits::BOX_SIZE; ++colIdx)
                            {
                                if( hasCandidate( grid[rowIdx][colIdx], val))
                                {
                                    cells.emplace_back( rowIdx, colIdx);
                                }
                            }
                        }

                        if( cells.size() < 2)
                        {
                            continue;  // size 0: undetectable here; size 1: hidden single
                        }

                        //
                        // All in the same row → eliminate from rest of that row.
                        //
                        const bool allSameRow = std::all_of( cells.begin(), cells.end(),
                            [&]( const Square& s) { return s.first == cells[0].first; });

                        if( allSameRow)
                        {
                            const int row = cells[0].first;

                            for( int colIdx : Traits::INDEX_RANGE)
                            {
                                if( colIdx < boxCol || colIdx >= boxCol + Traits::BOX_SIZE)
                                {
                                    progress |= removeFrom( grid[row][colIdx], val);
                                }
                            }
                        }

                        //
                        // All in the same column → eliminate from rest of that column.
                        //
                        const bool allSameCol = std::all_of( cells.begin(), cells.end(),
                            [&]( const Square& s) { return s.second == cells[0].second; });

                        if( allSameCol)
                        {
                            const int col = cells[0].second;

                            for( int rowIdx : Traits::INDEX_RANGE)
                            {
                                if( rowIdx < boxRow || rowIdx >= boxRow + Traits::BOX_SIZE)
                                {
                                    progress |= removeFrom( grid[rowIdx][col], val);
                                }
                            }
                        }
                    }
                }
            }

            return progress;
        }

        //
        // Rule 6: Box-Line Reduction
        //
        // When all occurrences of a value inside a row or column lie within one box,
        // eliminate that value from the rest of that box.
        //
        auto applyBoxLineReduction( CandidateGrid& grid) -> bool
        {
            bool progress = false;

            //
            // Check each row: if all occurrences of val are confined to one box.
            //
            for( int rowIdx : Traits::INDEX_RANGE)
            {
                for( const Traits::Value value : Traits::VALUE_RANGE)
                {
                    std::vector<Square> cells;

                    for( int colIdx : Traits::INDEX_RANGE)
                    {
                        if( hasCandidate( grid[rowIdx][colIdx], value))
                        {
                            cells.emplace_back( rowIdx, colIdx);
                        }
                    }

                    if( cells.size() < 2)
                    {
                        continue;
                    }

                    const int  boxCol     = (cells[0].second / Traits::BOX_SIZE) * Traits::BOX_SIZE;

                    const bool allSameBox = std::all_of( cells.begin(), cells.end(),
                        [&]( const Square& s)
                        {
                            return (s.second / Traits::BOX_SIZE) * Traits::BOX_SIZE == boxCol;
                        });

                    if( allSameBox)
                    {
                        const int boxRow = (rowIdx / Traits::BOX_SIZE) * Traits::BOX_SIZE;

                        for( int row = boxRow; row < boxRow + Traits::BOX_SIZE; ++row)
                        {
                            if( row != rowIdx)
                            {
                                for( int col = boxCol; col < boxCol + Traits::BOX_SIZE; ++col)
                                {
                                    progress |= removeFrom( grid[row][col], value);
                                }
                            }
                        }
                    }
                }
            }

            //
            // Check each column: if all occurrences of val are confined to one box.
            //
            for( int colIdx : Traits::INDEX_RANGE)
            {
                for( const Traits::Value val : Traits::VALUE_RANGE)
                {
                    std::vector<Square> cells;

                    for( int rowIdx : Traits::INDEX_RANGE)
                    {
                        if( hasCandidate( grid[rowIdx][colIdx], val))
                        {
                            cells.emplace_back( rowIdx, colIdx);
                        }
                    }

                    if( cells.size() < 2) continue;

                    const int boxRow = (cells[0].first / Traits::BOX_SIZE) * Traits::BOX_SIZE;

                    const bool allSameBox = std::all_of( cells.begin(), cells.end(),
                        [&]( const Square& s)
                        {
                            return (s.first / Traits::BOX_SIZE) * Traits::BOX_SIZE == boxRow;
                        });

                    if( allSameBox)
                    {
                        const int boxCol = (colIdx / Traits::BOX_SIZE) * Traits::BOX_SIZE;

                        for( int row = boxRow; row < boxRow + Traits::BOX_SIZE; ++row)
                        {
                            for( int col = boxCol; col < boxCol + Traits::BOX_SIZE; ++col)
                            {
                                if( col != colIdx)
                                {
                                    progress |= removeFrom( grid[row][col], val);
                                }
                            }
                        }
                    }
                }
            }

            return progress;
        }

    }

    //
    // RuleBasedSolver
    //
    RuleBasedSolver::RuleBasedSolver( const Logger& logger)
        : ISolver( logger)
    {}

    auto RuleBasedSolver::solve( Traits::Board& board, std::atomic<bool>& cancelFlag) const -> Traits::BoardResult
    {
        size_t passes{ 0};

        try
        {
            auto result = detail::initCandidates( board);

            if( ! result)
            {
                mLogger << result.error() << std::endl;

                return { false, passes, Traits::Board{}};
            }

            auto grid = std::move( *result);

            if( detail::isContradicted( grid))
            {
                return { false, passes, Traits::Board{}};
            }

            //
            // Already solved (all cells are givens or trivially resolved during init).
            //
            if( detail::isSolved( grid))
            {
                board = detail::toBoard( grid);

                cancelFlag.store( true);

                return { true, passes, board};
            }

            bool progress = true;

            while( progress)
            {
                if( cancelFlag.load()) throw CancelledException{};

                ++passes;

                progress  = detail::applyNakedSingles     ( grid);
                progress |= detail::applyHiddenSingles    ( grid);
                progress |= detail::applyNakedPairs       ( grid);
                progress |= detail::applyHiddenPairs      ( grid);
                progress |= detail::applyPointingPairs    ( grid);
                progress |= detail::applyBoxLineReduction ( grid);

                if( detail::isContradicted( grid))
                {
                    return { false, passes, Traits::Board{}};
                }

                if( detail::isSolved( grid))
                {
                    board = detail::toBoard( grid);
                    cancelFlag.store( true);

                    return { true, passes, board};
                }
            }

            //
            // No rule yielded further progress — the board requires guessing.
            //
            return { false, passes, Traits::Board{}};
        }
        catch( const CancelledException&)
        {
            return { false, passes, Traits::Board{}};
        }
    }
}