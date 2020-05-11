#ifndef PROYECTO1_RANDOMFILE_H
#define PROYECTO1_RANDOMFILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "Parser.h"
#include "Record.h"

class RandomFile {
private:
    std::string indexFile;
    std::string dataFile;

    void generateIndex() {
        std::cout << "\n** init Index File **\n";
        std::ofstream outIndexFile;
        outIndexFile.open(indexFile,std::ios::out | std::ios::trunc | std::ios::binary); // trunc 'discard existing content'
        std::ifstream inFile;
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        Record record;
        while(inFile >> record) {
            record.showData();
        }
        outIndexFile.close();
    }

public:
    // Default Constructor
    RandomFile() = default;

    // Custom Constructor
    RandomFile(std::string _dataFile){
        std::cout << "Custom Constructor\n";
        dataFile = _dataFile;
        indexFile = "../indexRandom";
        generateIndex();
    }
};

#endif //PROYECTO1_RANDOMFILE_H
