#include "CompressorStation.h"

CompressorStation::CompressorStation()
    : id(0), workshops(0), workingWorkshops(0), stationClass(0) {}

CompressorStation::CompressorStation(int id, const std::string& name,
                                     int workshops, int workingWorkshops,
                                     int stationClass)
    : id(id), name(name), workshops(workshops),
      workingWorkshops(workingWorkshops), stationClass(stationClass) {}

int CompressorStation::getId() const { return id; }
const std::string& CompressorStation::getName() const { return name; }
int CompressorStation::getWorkshops() const { return workshops; }
int CompressorStation::getWorkingWorkshops() const { return workingWorkshops; }
int CompressorStation::getStationClass() const { return stationClass; }

double CompressorStation::getInactivePercent() const {
    return 100.0 * (workshops - workingWorkshops) / workshops;
}

bool CompressorStation::startWorkshop() {
    if (workingWorkshops >= workshops) return false;
    workingWorkshops++;
    return true;
}

bool CompressorStation::stopWorkshop() {
    if (workingWorkshops <= 0) return false;
    workingWorkshops--;
    return true;
}

void CompressorStation::print() const {
    std::cout << "\nStation ID: " << id << '\n'
              << "Name: " << name << '\n'
              << "Workshops: " << workshops << '\n'
              << "Working workshops: " << workingWorkshops << '\n'
              << "Inactive workshops: " << getInactivePercent() << "%\n"
              << "Class: " << stationClass << '\n';
}

std::ostream& operator<<(std::ostream& out,
                         const CompressorStation& station) {
    out << station.id << '\n'
        << station.name << '\n'
        << station.workshops << '\n'
        << station.workingWorkshops << '\n'
        << station.stationClass << '\n';
    return out;
}

std::istream& operator>>(std::istream& in, CompressorStation& station) {
    int id, workshops, workingWorkshops, stationClass;
    std::string name;

    if (!(in >> id)) return in;
    in.ignore(10000, '\n');
    if (!std::getline(in, name)) return in;
    if (!(in >> workshops >> workingWorkshops >> stationClass)) return in;
    in.ignore(10000, '\n');

    if (id <= 0 || name.empty() || workshops <= 0 ||
        workingWorkshops < 0 || workingWorkshops > workshops ||
        stationClass < 0) {
        in.setstate(std::ios::failbit);
        return in;
    }

    station = CompressorStation(id, name, workshops,
                                workingWorkshops, stationClass);
    return in;
}
