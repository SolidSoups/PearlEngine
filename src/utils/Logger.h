#pragma once

#ifndef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#endif

#ifndef LOG_TO_CONSOLE
#define LOG_TO_CONSOLE 1
#endif

#include <iostream>
#include <vector>
#include <sstream>

// Log Entry definition
enum LogSeverity { INFO, WARNING, ERROR };
struct LogEntry {
  std::string message;
  LogSeverity severity;
  std::string file;
  int line;
  std::string function;
};

// 
class Logger {
public:
  void LogMessage(const std::string& msg, LogSeverity severity, 
                  const char* file, int line, const char* func) {
    m_Logs.push_back({msg, severity, file, line, func});
#if LOG_TO_CONSOLE
    if(severity == LogSeverity::ERROR)
      std::cerr << GetLogFormattedText(m_Logs[m_Logs.size()-1]) << std::endl;
    else
      std::cout << GetLogFormattedText(m_Logs[m_Logs.size()-1]) << std::endl;
#endif
  }

  const std::string GetLogFormattedText(const LogEntry& entry){
    std::stringstream ss;
    ss << "[" << GetSevText(entry.severity) << "] " 
       << entry.file << ":" << entry.line << " "
       << entry.function << " --- "
       << entry.message;
    return ss.str();
  }

  static Logger &Get() {
    static Logger instance;
    return instance;
  }

  void ClearLog(){
    m_Logs.clear();
  }

  const std::vector<LogEntry>& GetLogs() const { return m_Logs; }

  static const char *GetSevText(LogSeverity sev) {
    switch (sev) {
      case INFO:
        return "INFO";
      case WARNING:
        return "WARNING";
      case ERROR:
        return "ERROR";
    }

    return "undefined";
  }
private:
  std::vector<LogEntry> m_Logs;
};


// The LogStream allows us to pipe in text, numbers, and other data formats
// into a stream so that it can easily be logged. Very simple wrapper for 
// a stringstream that when destroyed, logs to the console
class LogStream {
public:
  LogStream(LogSeverity sev, const char* file, int line, const char* func) 
  : m_Severity(sev)
  , m_File(file)
  , m_Line(line)
  , m_Function(func)
  {}

  template<typename T>
  LogStream& operator<<(const T& value){
    m_ss << value;
    return *this;
  }

  ~LogStream(){ Logger::Get().LogMessage(m_ss.str(), m_Severity, m_File, m_Line, m_Function); }

private:
  std::stringstream m_ss;
  const char* m_File;
  int m_Line;
  const char* m_Function;
  LogSeverity m_Severity;
};

// Macros are defined here
#if DEBUG_ENABLED
#define LOG_INFO LogStream(LogSeverity::INFO, __FILE__, __LINE__, __func__)
#define LOG_WARNING LogStream(LogSeverity::WARNING, __FILE__, __LINE__, __func__)
#define LOG_ERROR LogStream(LogSeverity::ERROR, __FILE__, __LINE__, __func__)
#else
#define LOG_INFO if(0) LogStream(LogSeverity::INFO, "", 0, "")
#define LOG_WARNING if(0) LogStream(LogSeverity::WARNING, "", 0, "")
#define LOG_ERROR if(0) LogStream(LogSeverity::ERROR, "", 0, "")
#endif
