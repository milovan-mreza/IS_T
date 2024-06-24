#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

// Define the Racun class
class Racun {
protected:
    std::string nazivBanke;
    std::string tipKartice;
    int brojRacuna;
    int godinaVazenja;

public:
    Racun(const std::string& naziv, const std::string& tip, int broj, int godina = 2023)
        : nazivBanke(naziv), tipKartice(tip), brojRacuna(broj), godinaVazenja(godina) {}

    virtual ~Racun() {}

    virtual bool sifraRacunaValidna() const = 0;

    // Getter for godinaVazenja
    int getGodinaVazenja() const {
        return godinaVazenja;
    }

    friend std::ostream& operator<<(std::ostream& os, const Racun& racun);
};

// Implement the operator<< for Racun class
std::ostream& operator<<(std::ostream& os, const Racun& racun) {
    os << racun.nazivBanke << " " << racun.tipKartice << " " << racun.brojRacuna << " " << racun.godinaVazenja;
    return os;
}

// Define the DevizniRacun class inheriting from Racun
class DevizniRacun : public Racun {
public:
    using Racun::Racun;

    bool sifraRacunaValidna() const override {
        std::string brojStr = std::to_string(brojRacuna);
        return std::all_of(brojStr.begin(), brojStr.end(), [](char c) {
            return std::isdigit(c) || std::islower(c);
            });
    }
};

// Define the DinarskiRacun class inheriting from Racun
class DinarskiRacun : public Racun {
public:
    using Racun::Racun;

    bool sifraRacunaValidna() const override {
        std::string brojStr = std::to_string(brojRacuna);
        return std::none_of(brojStr.begin(), brojStr.end(), [](char c) {
            return !(std::isdigit(c) || std::isupper(c));
            });
    }
};

int main() {
    // Vector to store accounts
    std::vector<Racun*> racuni;

    // Open input file
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file!" << std::endl;
        return 1;
    }

    // Read each line from input file and create corresponding objects
    std::string line;
    bool firstLine = true;
    // Read each line from input file and create corresponding objects
    std::cout << "\n";
    while (std::getline(inputFile, line)) {
        std::cout << "|" << line;
        if (!line.empty() && line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF') {
            line.erase(0, 3); // Erase the BOM characters
        }

        char oznaka;
        std::string naziv, tip;
        int broj, godina;

        std::istringstream iss(line);
        iss >> oznaka >> naziv >> tip >> broj >> godina;

        if (godina == 0) {
            godina = 2023; // Default year
        }
        else
        {
            std::cout << std::endl << std::endl;
            std::cout << "[" << oznaka << "]";
            std::cout << std::endl << std::endl;
        }

        // Create objects based on the type (I for DevizniRacun, D for DinarskiRacun)
        if (oznaka == 'I') {
            racuni.push_back(new DevizniRacun(naziv, tip, broj, godina));
        }
        else if (oznaka == 'D') {
            racuni.push_back(new DinarskiRacun(naziv, tip, broj, godina));
        }
        else {
            std::cerr << "Unknown account type: " << oznaka << std::endl << std::endl;
            // Handle or skip unknown types as necessary
        }

      
    }


    // Close input file
    inputFile.close();

    // Sort accounts based on godinaVazenja (year of validity) in descending order
    std::stable_sort(racuni.begin(), racuni.end(), [](const Racun* a, const Racun* b) {
        return a->getGodinaVazenja() < b->getGodinaVazenja();
        });

    // Output sorted accounts
    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file!" << std::endl;
        return 1;
    }

    for (const auto& racun : racuni) {
        outputFile << *racun << std::endl;
    }

    // Close output file
    outputFile.close();

    // Clean up allocated memory
    for (auto racun : racuni) {
        delete racun;
    }

    return 0;
}
