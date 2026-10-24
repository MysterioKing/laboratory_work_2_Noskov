#pragma once
#include <map>
#include <vector>
#include "CompressorStation.h"
#include "Pipe.h"

class Manager {
private:
    std::map<int, Pipe> pipes;
    std::map<int, CompressorStation> stations;
    int nextPipeId = 1;
    int nextStationId = 1;

    void printMenu() const;
    void addPipe();
    void addStation();
    void showAll() const;
    void editPipe();
    void editStation();
    void deletePipe();
    void deleteStation();
    void searchPipes();
    void searchStations() const;
    void processFoundPipes(const std::vector<int>& foundIds);
    std::vector<int> selectPipes(const std::vector<int>& foundIds) const;
    void save() const;
    void load();
public:
    void run();
};
