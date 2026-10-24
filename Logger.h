#pragma once
#include <fstream>
#include <string>

class Logger {
private:
    std::ofstream file;
    Logger();
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    static Logger& getInstance();
    void write(const std::string& message);
};
