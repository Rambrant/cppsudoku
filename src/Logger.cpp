//
// Created by Thomas Rambrant on 2025-05-12.
//
#include "Logger.hpp"

namespace com::rambrant::sudoku
{
    Logger::Logger( const LogLevel defaultLevel, std::ostream& stream) :
        mDefaultLevel( defaultLevel),
        mCurrentLevel( defaultLevel),
        mOut( stream)
    {}

    auto Logger::isVerbose() const -> bool
    {
        return mCurrentLevel == LogLevel::Verbose;
    }

    auto Logger::operator<<( const LevelManipulator & manip ) const -> const Logger&
    {
        mCurrentLevel = manip.mLevel;
        return *this;
    }

    auto Logger::operator<<( std::ostream & (*manip)( std::ostream & )) const -> const Logger&
    {
        if( shouldPrint())
            mOut << manip;

        mCurrentLevel = mDefaultLevel; // reset after each std::endl or similar

        return *this;
    }

    auto Logger::shouldPrint() const -> bool
    {
        if( mDefaultLevel == LogLevel::Quiet)
            return false;

        return static_cast<int>( mCurrentLevel) <= static_cast<int>( mDefaultLevel);
    }
}