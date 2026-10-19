#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Pipe {
    string name;
    double length = 0;
    int diameter = 0;
    bool repair = false;
};

struct CompressorStation {
    string name;
    int workshops = 0;
    int workingWorkshops = 0;
    int stationClass = 0;
};

int readInt(string message, int minValue, int maxValue) {
    int number;

    while (true) {
        cout << message;
        cin >> number;

        if (cin && cin.peek() == '\n' &&
            number >= minValue && number <= maxValue) {
            cin.ignore(10000, '\n');
            return number;
        }

        if (cin.eof()) return -1;

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Wrong number. Try again.\n";
    }
}

double readLength() {
    double length;

    while (true) {
        cout << "Length (km): ";
        cin >> length;

        if (cin && cin.peek() == '\n' &&
            length > 0 && length < 1e100) {
            cin.ignore(10000, '\n');
            return length;
        }

        if (cin.eof()) return -1;

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Wrong length. Try again.\n";
    }
}

void inputPipe(Pipe& pipe) {
    cout << "\nPipe name: ";
    getline(cin, pipe.name);

    while (pipe.name.empty()) {
        cout << "Name cannot be empty. Enter again: ";
        getline(cin, pipe.name);
    }

    pipe.length = readLength();
    pipe.diameter = readInt("Diameter (mm): ", 1, 1000000);
    pipe.repair = readInt("Under repair? (0-no, 1-yes): ", 0, 1);

    cout << "Pipe added.\n";
}

void inputStation(CompressorStation& station) {
    cout << "\nStation name: ";
    getline(cin, station.name);

    while (station.name.empty()) {
        cout << "Name cannot be empty. Enter again: ";
        getline(cin, station.name);
    }

    station.workshops = readInt("Number of workshops: ", 1, 10000);
    station.workingWorkshops =
        readInt("Working workshops: ", 0, station.workshops);
    station.stationClass = readInt("Station class: ", 0, 10000);

    cout << "Station added.\n";
}

void printPipe(Pipe pipe) {
    cout << "\nPipe: " << pipe.name << '\n';
    cout << "Length: " << pipe.length << " km\n";
    cout << "Diameter: " << pipe.diameter << " mm\n";
    cout << "Under repair: " << (pipe.repair ? "Yes" : "No") << '\n';
}

void printStation(CompressorStation station) {
    cout << "\nStation: " << station.name << '\n';
    cout << "Workshops: " << station.workshops << '\n';
    cout << "Working workshops: " << station.workingWorkshops << '\n';
    cout << "Class: " << station.stationClass << '\n';
}

void editPipe(Pipe& pipe) {
    pipe.repair = readInt("\nNew repair status (0-no, 1-yes): ", 0, 1);
    cout << "Pipe changed.\n";
}

void editStation(CompressorStation& station) {
    int choice = readInt(
        "\n1. Start workshop\n2. Stop workshop\n0. Cancel\nChoice: ",
        0, 2
    );

    if (choice == 1) {
        if (station.workingWorkshops < station.workshops) {
            station.workingWorkshops++;
            cout << "Workshop started.\n";
        }
        else {
            cout << "All workshops are already working.\n";
        }
    }
    else if (choice == 2) {
        if (station.workingWorkshops > 0) {
            station.workingWorkshops--;
            cout << "Workshop stopped.\n";
        }
        else {
            cout << "No working workshops.\n";
        }
    }
}

void savePipe(ofstream& file, Pipe pipe) {
    file << pipe.name << '\n';
    file << pipe.length << '\n';
    file << pipe.diameter << '\n';
    file << pipe.repair << '\n';
}

void saveStation(ofstream& file, CompressorStation station) {
    file << station.name << '\n';
    file << station.workshops << '\n';
    file << station.workingWorkshops << '\n';
    file << station.stationClass << '\n';
}

void saveData(Pipe pipe, bool pipeExists,
    CompressorStation station, bool stationExists) {
    if (!pipeExists && !stationExists) {
        cout << "Nothing to save.\n";
        return;
    }

    ofstream file("data.txt");

    if (!file) {
        cout << "Cannot open data.txt.\n";
        return;
    }

    file << pipeExists << '\n';
    if (pipeExists) savePipe(file, pipe);

    file << stationExists << '\n';
    if (stationExists) saveStation(file, station);

    file.close();
    cout << "\nData saved.\n";
}

bool loadPipe(ifstream& file, Pipe& pipe) {
    int repair;

    if (!getline(file, pipe.name) ||
        !(file >> pipe.length >> pipe.diameter >> repair))
        return false;

    file.ignore(10000, '\n');

    if (pipe.name.empty() || pipe.length <= 0 ||
        pipe.length >= 1e100 || pipe.diameter <= 0 ||
        (repair != 0 && repair != 1))
        return false;

    pipe.repair = repair;
    return true;
}

bool loadStation(ifstream& file, CompressorStation& station) {
    if (!getline(file, station.name) ||
        !(file >> station.workshops
            >> station.workingWorkshops
            >> station.stationClass))
        return false;

    if (station.name.empty() || station.workshops <= 0 ||
        station.workingWorkshops < 0 ||
        station.workingWorkshops > station.workshops ||
        station.stationClass < 0)
        return false;

    return true;
}

void loadData(Pipe& pipe, bool& pipeExists,
    CompressorStation& station, bool& stationExists) {
    ifstream file("data.txt");

    if (!file) {
        cout << "data.txt not found.\n";
        return;
    }

    int hasPipe, hasStation;

    if (!(file >> hasPipe) || (hasPipe != 0 && hasPipe != 1)) {
        cout << "Invalid file.\n";
        return;
    }
    file.ignore(10000, '\n');

    if (hasPipe && !loadPipe(file, pipe)) {
        cout << "Invalid pipe data.\n";
        return;
    }

    if (!(file >> hasStation) || (hasStation != 0 && hasStation != 1)) {
        cout << "Invalid file.\n";
        return;
    }
    file.ignore(10000, '\n');

    if (hasStation && !loadStation(file, station)) {
        cout << "Invalid station data.\n";
        return;
    }

    pipeExists = (hasPipe == 1);
    stationExists = (hasStation == 1);

    cout << "\nData loaded.\n";
}

int main() {
    Pipe pipe;
    CompressorStation station;
    bool pipeExists = false;
    bool stationExists = false;

    while (true) {
        cout << "\n1. Add pipe\n";
        cout << "2. Add station\n";
        cout << "3. View all objects\n";
        cout << "4. Edit pipe\n";
        cout << "5. Edit station\n";
        cout << "6. Save\n";
        cout << "7. Load\n";
        cout << "0. Exit\n";

        int command = readInt("\nChoice: ", 0, 7);
        if (command == -1) return 0;

        switch (command) {
        case 0:
            return 0;

        case 1:
            inputPipe(pipe);
            pipeExists = true;
            break;

        case 2:
            inputStation(station);
            stationExists = true;
            break;

        case 3:
            if (pipeExists) printPipe(pipe);
            else cout << "Pipe not added.\n";

            if (stationExists) printStation(station);
            else cout << "Station not added.\n";
            break;

        case 4:
            if (pipeExists) editPipe(pipe);
            else cout << "Pipe not added.\n";
            break;

        case 5:
            if (stationExists) editStation(station);
            else cout << "Station not added.\n";
            break;

        case 6:
            saveData(pipe, pipeExists, station, stationExists);
            break;

        case 7:
            loadData(pipe, pipeExists, station, stationExists);
            break;
        }
    }
}