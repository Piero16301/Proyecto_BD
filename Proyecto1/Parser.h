#ifndef PROYECTO1_PARSER_H
#define PROYECTO1_PARSER_H

#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <cstring>

#include "Record.h"

using namespace std;

class Parser {
private:
    string csvFile;
    string binFile;

public:
    explicit Parser(string file, string binary) : csvFile{std::move(file)}, binFile{std::move(binary)} {}

    void csvToBinaryFile() {
        ifstream file(csvFile);
        if (file.is_open()) {
            string fields[4], row;
            getline(file, row); //Skip field titles
            fstream binaryFile;
            binaryFile.open(binFile, std::ios::out | std::ios::binary);
            while (!file.eof()) {
                getline(file, row);
                istringstream istringstream1(row);
                unsigned int counter = 0;
                while (getline(istringstream1, row, ',')) {
                    fields[counter] = row;
                    counter++;
                }
                Record record(stoi(fields[0]), fields[1].c_str(), fields[2].c_str(), fields[3].c_str());
                binaryFile << record;
            }
            binaryFile.close();
        }
    }
};

#endif //PROYECTO1_PARSER_H