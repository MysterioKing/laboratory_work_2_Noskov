#include "Utils.h"
#include <cstdlib>
#include <iostream>

int readInt(const std::string& message, int minValue, int maxValue) {
    int value;
    while (true) {
        std::cout << message;
        std::cin >> value;
        if (std::cin.eof()) std::exit(0);
        if (std::cin && std::cin.peek() == '\n' &&
            value >= minValue && value <= maxValue) {
            std::cin.ignore(10000, '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Enter an integer from " << minValue << " to " << maxValue << ".\n";
    }
}

double readDouble(const std::string& message, double minValue, double maxValue) {
    double value;
    while (true) {
        std::cout << message;
        std::cin >> value;
        if (std::cin.eof()) std::exit(0);
        if (std::cin && std::cin.peek() == '\n' &&
            value >= minValue && value <= maxValue) {
            std::cin.ignore(10000, '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Enter a number from " << minValue << " to " << maxValue << ".\n";
    }
}

std::string readLine(const std::string& message) {
    std::string value;
    while (value.find_first_not_of(" \t") == std::string::npos) {
        std::cout << message;
        if (!std::getline(std::cin, value)) std::exit(0);
        if (value.find_first_not_of(" \t") == std::string::npos)
            std::cout << "Value cannot be empty.\n";
    }
    return value;
}
