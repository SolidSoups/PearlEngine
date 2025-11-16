#pragma once

#ifndef LOG_TO_CONSOLE
#define DEBUG_ENABLED 1
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
};

// 
class Logger {
public:
  void LogMessage(const std::string& msg, LogSeverity severity) {
    m_Logs.push_back({msg, severity});
#if LOG_TO_CONSOLE == 1
    std::cout << "[DEBUG]" << "-[" << GetSevText(severity) << "]: " << msg << "\n";
#endif
  }

  const std::string GetLogFormattedText(const LogEntry& entry){
    std::stringstream ss;
    ss << "[" << GetSevText(entry.severity) << "] " << entry.message;
    return ss.str();
  }

  static Logger &Get() {
    static Logger instance;
    return instance;
  }

  const std::vector<LogEntry>& GetLogs() const { return m_Logs; }

private:
  std::vector<LogEntry> m_Logs;
  const char *GetSevText(LogSeverity sev) {
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
};


// The LogStream allows us to pipe in text, numbers, and other data formats
// into a stream so that it can easily be logged. Very simple wrapper for 
// a stringstream that when destroyed, logs to the console
class LogStream {
public:
  LogStream(LogSeverity sev) : severity(sev){}

  template<typename T>
  LogStream& operator<<(const T& value){
    ss << value;
    return *this;
  }

  ~LogStream(){ Logger::Get().LogMessage(ss.str(), severity); }

private:
  std::stringstream ss;
  LogSeverity severity;
};

// Macros are defined here
#ifdef DEBUG_ENABLED
#define LOG_INFO LogStream(LogSeverity::INFO)
#define LOG_WARNING LogStream(LogSeverity::WARNING)
#define LOG_ERROR LogStream(LogSeverity::ERROR)
#endif
