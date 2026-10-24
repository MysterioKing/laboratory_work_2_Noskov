#include "Manager.h"

#include "Logger.h"
#include "Utils.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

namespace {
bool containsId(const std::vector<int>& ids, int id) {
    for (int value : ids)
        if (value == id) return true;
    return false;
}
}

void Manager::printMenu() const {
    std::cout << "\n1. Add pipe\n"
              << "2. Add station\n"
              << "3. View all objects\n"
              << "4. Edit pipe by ID\n"
              << "5. Edit station by ID\n"
              << "6. Delete pipe by ID\n"
              << "7. Delete station by ID\n"
              << "8. Search pipes\n"
              << "9. Search stations\n"
              << "10. Save\n"
              << "11. Load\n"
              << "0. Exit\n";
}

void Manager::addPipe() {
    std::string name = readLine("Pipe name: ");
    double length = readDouble("Length (km): ", 0.01, 1000000);
    int diameter = readInt("Diameter (mm): ", 1, 1000000);
    bool repairing = readInt("Under repair? (0-no, 1-yes): ", 0, 1);

    Pipe pipe(nextPipeId, name, length, diameter, repairing);
    pipes.emplace(nextPipeId, pipe);
    std::cout << "Pipe added. ID: " << nextPipeId << '\n';
    Logger::getInstance().write("Added pipe ID " + std::to_string(nextPipeId));
    nextPipeId++;
}

void Manager::addStation() {
    std::string name = readLine("Station name: ");
    int workshops = readInt("Number of workshops: ", 1, 10000);
    int working = readInt("Working workshops: ", 0, workshops);
    int stationClass = readInt("Station class: ", 0, 10000);

    CompressorStation station(nextStationId, name, workshops,
                              working, stationClass);
    stations.emplace(nextStationId, station);
    std::cout << "Station added. ID: " << nextStationId << '\n';
    Logger::getInstance().write("Added station ID " + std::to_string(nextStationId));
    nextStationId++;
}

void Manager::showAll() const {
    if (pipes.empty()) std::cout << "\nNo pipes.\n";
    for (const auto& item : pipes) item.second.print();

    if (stations.empty()) std::cout << "\nNo stations.\n";
    for (const auto& item : stations) item.second.print();

    Logger::getInstance().write("Viewed all objects");
}

void Manager::editPipe() {
    int id = readInt("Pipe ID: ", 1, 1000000000);
    auto item = pipes.find(id);

    if (item == pipes.end()) {
        std::cout << "Pipe not found.\n";
        Logger::getInstance().write("Pipe ID " + std::to_string(id) + " not found");
        return;
    }

    bool value = readInt("New repair status (0-no, 1-yes): ", 0, 1);
    item->second.setRepairing(value);
    std::cout << "Pipe changed.\n";
    Logger::getInstance().write("Edited pipe ID " + std::to_string(id));
}

void Manager::editStation() {
    int id = readInt("Station ID: ", 1, 1000000000);
    auto item = stations.find(id);

    if (item == stations.end()) {
        std::cout << "Station not found.\n";
        Logger::getInstance().write("Station ID " + std::to_string(id) + " not found");
        return;
    }

    int action = readInt("1. Start workshop\n2. Stop workshop\n0. Cancel\nChoice: ", 0, 2);

    if (action == 1) {
        if (item->second.startWorkshop()) {
            std::cout << "Workshop started.\n";
            Logger::getInstance().write("Started workshop at station ID " + std::to_string(id));
        } else {
            std::cout << "All workshops are already working.\n";
            Logger::getInstance().write("Could not start workshop at station ID " + std::to_string(id));
        }
    } else if (action == 2) {
        if (item->second.stopWorkshop()) {
            std::cout << "Workshop stopped.\n";
            Logger::getInstance().write("Stopped workshop at station ID " + std::to_string(id));
        } else {
            std::cout << "No working workshops.\n";
            Logger::getInstance().write("Could not stop workshop at station ID " + std::to_string(id));
        }
    } else {
        Logger::getInstance().write("Cancelled station editing");
    }
}

void Manager::deletePipe() {
    int id = readInt("Pipe ID: ", 1, 1000000000);

    if (pipes.erase(id) == 0) {
        std::cout << "Pipe not found.\n";
        Logger::getInstance().write("Pipe ID " + std::to_string(id) + " not found");
    } else {
        std::cout << "Pipe deleted.\n";
        Logger::getInstance().write("Deleted pipe ID " + std::to_string(id));
    }
}

void Manager::deleteStation() {
    int id = readInt("Station ID: ", 1, 1000000000);

    if (stations.erase(id) == 0) {
        std::cout << "Station not found.\n";
        Logger::getInstance().write("Station ID " + std::to_string(id) + " not found");
    } else {
        std::cout << "Station deleted.\n";
        Logger::getInstance().write("Deleted station ID " + std::to_string(id));
    }
}

std::vector<int> Manager::selectPipes(const std::vector<int>& foundIds) const {
    int mode = readInt("1. Select all\n2. Select by ID\nChoice: ", 1, 2);
    if (mode == 1) return foundIds;

    std::vector<int> selected;
    int count = readInt("Number of pipes: ", 1, static_cast<int>(foundIds.size()));

    while (static_cast<int>(selected.size()) < count) {
        int id = readInt("Pipe ID: ", 1, 1000000000);

        if (!containsId(foundIds, id))
            std::cout << "This ID is not in the search result.\n";
        else if (containsId(selected, id))
            std::cout << "This ID is already selected.\n";
        else
            selected.push_back(id);
    }

    return selected;
}

void Manager::processFoundPipes(const std::vector<int>& foundIds) {
    int action = readInt("1. Change repair status\n2. Delete\n0. Return\nChoice: ", 0, 2);
    if (action == 0) return;

    std::vector<int> selected = selectPipes(foundIds);

    if (action == 1) {
        bool value = readInt("New repair status (0-no, 1-yes): ", 0, 1);
        for (int id : selected) pipes.at(id).setRepairing(value);
        std::cout << "Selected pipes changed.\n";
        Logger::getInstance().write("Batch edited " + std::to_string(selected.size()) + " pipes");
    } else {
        for (int id : selected) pipes.erase(id);
        std::cout << "Selected pipes deleted.\n";
        Logger::getInstance().write("Batch deleted " + std::to_string(selected.size()) + " pipes");
    }
}

void Manager::searchPipes() {
    if (pipes.empty()) {
        std::cout << "No pipes.\n";
        return;
    }

    int filter = readInt("1. By name\n2. By repair status\nChoice: ", 1, 2);
    std::vector<int> foundIds;

    if (filter == 1) {
        std::string text = readLine("Part of name: ");
        for (const auto& item : pipes)
            if (item.second.getName().find(text) != std::string::npos)
                foundIds.push_back(item.first);
        Logger::getInstance().write("Searched pipes by name: " + text);
    } else {
        bool repairing = readInt("Under repair? (0-no, 1-yes): ", 0, 1);
        for (const auto& item : pipes)
            if (item.second.isRepairing() == repairing)
                foundIds.push_back(item.first);
        Logger::getInstance().write("Searched pipes by repair status");
    }

    if (foundIds.empty()) {
        std::cout << "No pipes found.\n";
        return;
    }

    for (int id : foundIds) pipes.at(id).print();
    processFoundPipes(foundIds);
}

void Manager::searchStations() const {
    if (stations.empty()) {
        std::cout << "No stations.\n";
        return;
    }

    int filter = readInt("1. By name\n2. By inactive workshop percent\nChoice: ", 1, 2);
    bool found = false;

    if (filter == 1) {
        std::string text = readLine("Part of name: ");
        for (const auto& item : stations) {
            if (item.second.getName().find(text) != std::string::npos) {
                item.second.print();
                found = true;
            }
        }
        Logger::getInstance().write("Searched stations by name: " + text);
    } else {
        double percent = readDouble("Inactive workshops percent: ", 0, 100);
        for (const auto& item : stations) {
            if (std::fabs(item.second.getInactivePercent() - percent) < 0.01) {
                item.second.print();
                found = true;
            }
        }
        Logger::getInstance().write("Searched stations by inactive percent");
    }

    if (!found) std::cout << "No stations found.\n";
}

void Manager::save() const {
    std::string fileName = readLine("File name: ");
    std::ofstream file(fileName);

    if (!file) {
        std::cout << "Cannot open file.\n";
        Logger::getInstance().write("Could not save to " + fileName);
        return;
    }

    file << pipes.size() << '\n';
    for (const auto& item : pipes) file << item.second;

    file << stations.size() << '\n';
    for (const auto& item : stations) file << item.second;

    std::cout << "Data saved.\n";
    Logger::getInstance().write("Saved data to " + fileName);
}

void Manager::load() {
    std::string fileName = readLine("File name: ");
    std::ifstream file(fileName);

    if (!file) {
        std::cout << "File not found.\n";
        Logger::getInstance().write("Could not load " + fileName);
        return;
    }

    std::map<int, Pipe> loadedPipes;
    std::map<int, CompressorStation> loadedStations;
    int pipeCount, stationCount;

    if (!(file >> pipeCount) || pipeCount < 0) {
        std::cout << "Invalid file.\n";
        Logger::getInstance().write("Invalid file " + fileName);
        return;
    }
    file.ignore(10000, '\n');

    for (int i = 0; i < pipeCount; i++) {
        Pipe pipe;
        if (!(file >> pipe) || !loadedPipes.emplace(pipe.getId(), pipe).second) {
            std::cout << "Invalid pipe data.\n";
            Logger::getInstance().write("Invalid pipe data in " + fileName);
            return;
        }
    }

    if (!(file >> stationCount) || stationCount < 0) {
        std::cout << "Invalid station data.\n";
        Logger::getInstance().write("Invalid station data in " + fileName);
        return;
    }
    file.ignore(10000, '\n');

    for (int i = 0; i < stationCount; i++) {
        CompressorStation station;
        if (!(file >> station) ||
            !loadedStations.emplace(station.getId(), station).second) {
            std::cout << "Invalid station data.\n";
            Logger::getInstance().write("Invalid station data in " + fileName);
            return;
        }
    }

    pipes = loadedPipes;
    stations = loadedStations;
    nextPipeId = 1;
    nextStationId = 1;

    for (const auto& item : pipes)
        if (item.first >= nextPipeId) nextPipeId = item.first + 1;
    for (const auto& item : stations)
        if (item.first >= nextStationId) nextStationId = item.first + 1;

    std::cout << "Data loaded.\n";
    Logger::getInstance().write("Loaded data from " + fileName);
}

void Manager::run() {
    Logger::getInstance().write("Program started");

    while (true) {
        printMenu();
        int command = readInt("Choice: ", 0, 11);
        Logger::getInstance().write("Selected menu item " + std::to_string(command));

        switch (command) {
        case 0:
            Logger::getInstance().write("Program finished");
            return;
        case 1: addPipe(); break;
        case 2: addStation(); break;
        case 3: showAll(); break;
        case 4: editPipe(); break;
        case 5: editStation(); break;
        case 6: deletePipe(); break;
        case 7: deleteStation(); break;
        case 8: searchPipes(); break;
        case 9: searchStations(); break;
        case 10: save(); break;
        case 11: load(); break;
        }
    }
}
