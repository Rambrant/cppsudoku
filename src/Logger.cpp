//
// Created by Thomas Rambrant on 2025-05-12.
//

// Logger.cpp
#include "Logger.hpp"

namespace com::rambrant::sudoku
{
    Logger::Logger( const LogLevel defaultLevel) :
        mDefaultLevel( defaultLevel),
        mCurrentLevel( defaultLevel),
        mOut( std::cout)
    {}

    auto Logger::operator<<( const LevelManipulator & manip ) -> Logger&
    {
        mCurrentLevel = manip.mLevel;
        return *this;
    }

    auto Logger::operator<<( std::ostream & (*manip)( std::ostream & ) ) -> Logger&
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