#include "precomp.h"
#include "Logger.h"
#include <iomanip>

static HMODULE GetCurrentModule()
{
    return _AtlBaseModule.GetModuleInstance();
}

Logger::Logger() : m_IsInitialized(false)
{
    // Constructor is empty; initialization happens in Initialize()
}

Logger::~Logger()
{
    if (m_LogFile.is_open())
    {
        m_LogFile.close();
    }
}

Logger& Logger::Instance()
{
    static Logger instance;
    return instance;
}

void Logger::Initialize()
{
    //std::lock_guard<std::mutex> lock(m_Mutex);

    if (!m_IsInitialized)
    {
        // Open the log file
        char path[MAX_PATH];
        HMODULE hModule = GetCurrentModule();
        DWORD length = GetModuleFileNameA(hModule, path, MAX_PATH);
        if (length > 0 && length < MAX_PATH)
        {
            // Extract the directory from the full path
            std::string fullPath(path, length);
            size_t pos = fullPath.find_last_of("\\/");
            std::string directory = (pos != std::string::npos) ? fullPath.substr(0, pos + 1) : "";

            // Construct the log file path
            std::string logFilePath = directory + "log.txt";

            // Open the log file
            m_LogFile.open(logFilePath.c_str(), std::ios::out | std::ios::app);
            if (m_LogFile.is_open())
            {
                m_IsInitialized = true;
            }
            else
            {
                MessageBox(NULL, TEXT("Uh oh! Couldn't initialize logging."), NULL, 0);
            }
        }
    }
}

// Overload for manipulators
Logger& Logger::operator<<(LoggerManipulator manip)
{
    return manip(*this);
}

Logger& Logger::Flush()
{
    if (!m_IsInitialized)
    {
        Initialize();
    }

    //std::lock_guard<std::mutex> lock(m_Mutex);

    if (m_LogFile.is_open())
    {
        // Get current time
        m_LogFile << m_Buffer.str() << std::endl;

        // Clear the buffer
        m_Buffer.str("");
        m_Buffer.clear();
    }
    return *this;
}

// Manipulator function for std::endl equivalent
Logger& Endl(Logger& logger)
{
    return logger.Flush();
}
