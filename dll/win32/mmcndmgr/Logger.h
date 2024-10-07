#pragma once

//#include <mutex>
#include <fstream>
#include <string>
#include <sstream>

class Logger
{
public:
    static Logger& Instance();

    // Overloaded stream insertion operators
    template <typename T>
    Logger& operator<<(const T& value)
    {
        if (!m_IsInitialized)
        {
            Initialize();
        }

        //std::lock_guard<std::mutex> lock(m_Mutex);

        m_Buffer << value;
        return *this;
    }

    // Overload for manipulators like std::endl
    typedef Logger& (*LoggerManipulator)(Logger&);
    Logger& operator<<(LoggerManipulator manip);

    // Flush the buffer and write to the log file
    Logger& Flush();

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void Initialize();

    std::ofstream m_LogFile;
    //std::mutex m_Mutex;
    bool m_IsInitialized;
    std::ostringstream m_Buffer; // Buffer to hold the log message
};

// Manipulator function for std::endl equivalent
Logger& Endl(Logger& logger);
