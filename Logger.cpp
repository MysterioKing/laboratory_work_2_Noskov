#include "Logger.h"
#include <ctime>
#include <iomanip>

Logger::Logger() : file("log.txt", std::ios::app) {}

Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

void Logger::write(const std::string& message) {
    if (!file) return;
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    file << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S")
         << " | " << message << '\n';
    file.flush();
}
