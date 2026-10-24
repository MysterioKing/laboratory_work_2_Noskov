#include "Pipe.h"

Pipe::Pipe() : id(0), length(0), diameter(0), repairing(false) {}

Pipe::Pipe(int id, const std::string& name, double length,
           int diameter, bool repairing)
    : id(id), name(name), length(length),
      diameter(diameter), repairing(repairing) {}

int Pipe::getId() const { return id; }
const std::string& Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
int Pipe::getDiameter() const { return diameter; }
bool Pipe::isRepairing() const { return repairing; }

void Pipe::setRepairing(bool value) { repairing = value; }

void Pipe::print() const {
    std::cout << "\nPipe ID: " << id << '\n'
              << "Name: " << name << '\n'
              << "Length: " << length << " km\n"
              << "Diameter: " << diameter << " mm\n"
              << "Under repair: " << (repairing ? "Yes" : "No") << '\n';
}

std::ostream& operator<<(std::ostream& out, const Pipe& pipe) {
    out << pipe.id << '\n'
        << pipe.name << '\n'
        << pipe.length << '\n'
        << pipe.diameter << '\n'
        << pipe.repairing << '\n';
    return out;
}

std::istream& operator>>(std::istream& in, Pipe& pipe) {
    int id, diameter, repairing;
    double length;
    std::string name;

    if (!(in >> id)) return in;
    in.ignore(10000, '\n');
    if (!std::getline(in, name)) return in;
    if (!(in >> length >> diameter >> repairing)) return in;
    in.ignore(10000, '\n');

    if (id <= 0 || name.empty() || length <= 0 || diameter <= 0 ||
        (repairing != 0 && repairing != 1)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    pipe = Pipe(id, name, length, diameter, repairing == 1);
    return in;
}
