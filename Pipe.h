#pragma once

#include <iostream>
#include <string>

class Pipe {
private:
    int id;
    std::string name;
    double length;
    int diameter;
    bool repairing;

public:
    Pipe();
    Pipe(int id, const std::string& name, double length,
         int diameter, bool repairing);

    int getId() const;
    const std::string& getName() const;
    double getLength() const;
    int getDiameter() const;
    bool isRepairing() const;
    void setRepairing(bool value);
    void print() const;

    friend std::ostream& operator<<(std::ostream& out, const Pipe& pipe);
    friend std::istream& operator>>(std::istream& in, Pipe& pipe);
};
