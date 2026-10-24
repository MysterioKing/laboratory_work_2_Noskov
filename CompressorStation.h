#pragma once

#include <iostream>
#include <string>

class CompressorStation {
private:
    int id;
    std::string name;
    int workshops;
    int workingWorkshops;
    int stationClass;

public:
    CompressorStation();
    CompressorStation(int id, const std::string& name, int workshops,
                      int workingWorkshops, int stationClass);

    int getId() const;
    const std::string& getName() const;
    int getWorkshops() const;
    int getWorkingWorkshops() const;
    int getStationClass() const;
    double getInactivePercent() const;
    bool startWorkshop();
    bool stopWorkshop();
    void print() const;

    friend std::ostream& operator<<(std::ostream& out,
                                    const CompressorStation& station);
    friend std::istream& operator>>(std::istream& in,
                                    CompressorStation& station);
};
