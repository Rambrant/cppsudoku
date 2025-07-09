//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <iostream>

namespace com::rambrant::sudoku
{
    /**
     * @brief A std::ostream compatible logger that supports different levels of logging
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
             * @brief An internal manipulator to control the level of the messages printed on the stream
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
             * @param stream The output stream to write to. Defaults to std::clog
             */
            explicit Logger( LogLevel defaultLevel = LogLevel::Normal, std::ostream& stream = std::clog);

            /**
             * @brief Chack if the Logger is in a verbose state or not
             * @return true if the level i set to Verbose
             */
            [[nodiscard]]
            auto isVerbose() const -> bool;

            /**
             * @brief Stream output operator that takes one of the internal manipulators to set the level of the following message
             * @param manip the manipulator Logger::normal, Logger::verbose
             * @return The Logger to allow chaining of calls
             */
            auto operator<<( const LevelManipulator & manip ) const -> const Logger&;

            /**
             * @brief Stream output operator that takes standard library manipulator such as std::endl, std::flush. Resets the stream to the default level
             * @param manip
             * @return The Logger to allow chaining of calls
             */
            auto operator<<( std::ostream & (*manip)( std::ostream & ) ) const -> const Logger&;


            /**
             * @brief Stream output operator to print out any type that can be converted to std::string
             * @tparam T the actual Type of the object to print
             * @param value The value to print
             * @return The Logger to allow chaining of calls
             */
            template<typename T>
            auto operator<<( const T & value ) const -> const Logger&;

        private:
            LogLevel          mDefaultLevel;
            mutable LogLevel  mCurrentLevel;
            std::ostream&     mOut;

            [[nodiscard]]
            auto shouldPrint() const -> bool;
    };

    /// \cond DOXYGEN_SUPPRESS
    template<typename T>
    auto Logger::operator<<( const T & value ) const -> const Logger&
    {
        if( shouldPrint())
            mOut << value;
        return *this;
    }
    /// \endcond
}