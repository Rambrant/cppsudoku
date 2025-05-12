//
// Created by Thomas Rambrant on 2025-05-12.
//
#pragma once

#include <iostream>
#include <ostream>

namespace com::rambrant::sudoku
{
    /**
     * @brief A std::cout compatible logger that supports different levels of logging
     */
    class Logger
    {
        public:
            /**
             * @brief The different log levels that the logger can have as default level
             */
            enum class LogLevel
            {
                /**
                 * @brief Silence all output through the logger
                 */
                Quiet   = 0,
                /**
                 * @brief The default level of logging. Should be used for sparse output
                 */
                Normal  = 1,
                /**
                 * @brief Verbose and detailed logging mainly used for debugging
                 */
                Verbose = 2
            };

            /**
             * @brief An interan maniplulator to control the level of the messages printed onn the stream
             */
            struct LevelManipulator
            {
                /// @brief the manipulators loglevel
                LogLevel mLevel;
            };

            /**
             * @brief A stream manipulator that temporarily sets the message level to normal. This is the default
             */
            static constexpr LevelManipulator normal  { LogLevel::Normal };
            /**
             * @brief A stream manipulator that temporarily sets the message level to verbose
             */
            static constexpr LevelManipulator verbose { LogLevel::Verbose };


            /**
             * @brief Construct a logger with a specific level
             * @param defaultLevel The max level to allow the printing of output messages
             */
            explicit Logger( LogLevel defaultLevel = LogLevel::Normal);

            /**
             * @brief Stream output operator that takes one of the internal manipulators to set the level of the following message
             * @param manip the manipulator Logger::normal, Logger::verbose
             * @return The Logger to allow chaining of calls
             */
            auto operator<<( const LevelManipulator & manip ) -> Logger&;

            /**
             * @brief Stream output operator that takes standard library manipulator such as std::endl, std::flush. Resets the stream to the default level
             * @param manip
             * @return The Logger to allow chaining of calls
             */
            auto operator<<( std::ostream & (*manip)( std::ostream & ) ) -> Logger&;


            /**
             * @brief Stream output operator to print out any type that can be converted to std::string
             * @tparam T the actual Type of the object to print
             * @param value The value to print
             * @return The Logger to allow chaining of calls
             */
            template<typename T>
            auto operator<<( const T & value ) -> Logger&;

        private:
            LogLevel      mDefaultLevel;
            LogLevel      mCurrentLevel;
            std::ostream& mOut;

            [[nodiscard]]
            auto shouldPrint() const -> bool;
    };

    /// \cond DOXYGEN_SUPPRESS
    template<typename T>
    Logger& Logger::operator<<( const T& value)
    {
        if( shouldPrint())
            mOut << value;
        return *this;
    }
    /// \endcond
}