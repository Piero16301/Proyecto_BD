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
    int rowsIndexFile;
    std::map<int, int> indexRandomMap;

    void updateIndexFile(int code, int row){
        std::cout << code << "," << row << "\n";
        std::fstream outIndexFile;
        outIndexFile.open(indexFile, std::ios::out |std::ios::app | std::ios::binary);
        if(outIndexFile.is_open()){
            outIndexFile.write((char *)(&code),  sizeof(code));       // Write Key
            outIndexFile.write((char *)(&row), sizeof(row));    // Write Row number
            outIndexFile << "\n";
            outIndexFile << std::flush;
        }
        else{
            std::cout << "Error opening indexFile\n";
        }
        outIndexFile.close();
    }

    void generateIndex() {
        std::cout << "\n** init Index File **\n";
        std::ofstream outIndexFile;
        outIndexFile.open(indexFile,std::ios::out | std::ios::trunc | std::ios::binary); // trunc 'discard existing content'
        outIndexFile.close();
        std::ifstream inFile;
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        Record record;
        int row=1;
        while(inFile >> record) {
            updateIndexFile(record.getCode(), row);
            row++;
        }
        rowsIndexFile = row;
    }

    void loadIndexRandomFile(){
        std::cout << "\n** Load Index File to Memory RAM **\n";
        std::ifstream inFile;
        inFile.open(indexFile, std::ios::in | std::ios::binary);
        int keyCode;
        int rowDataFile;
        for(int i = 1; i <= rowsIndexFile; i++){
            inFile.read(reinterpret_cast<char *>(&keyCode), sizeof(rowDataFile));
            inFile.read(reinterpret_cast<char *>(&rowDataFile), sizeof(rowDataFile));
            indexRandomMap.insert({keyCode, rowDataFile});
            inFile.get();   // read endLine character
        }
        inFile.close();
    }

public:
    // Default Constructor
    RandomFile() = default;

    // Custom Constructor
    RandomFile(std::string _dataFile){
        std::cout << "Custom Constructor\n";
        dataFile = _dataFile;
        indexFile = "../indexRandom.dat";
        rowsIndexFile = 0;
        generateIndex();
        loadIndexRandomFile();
    }

    void showIndexRandomFile() {
        std::cout << "\n** Show Index File from disk **\n";
        std::ifstream inFile;
        inFile.open(indexFile, std::ios::in | std::ios::binary);
        int codeKey;
        int rowDataFile;
        for(int i = 1; i <= rowsIndexFile; i++){
            inFile.read(reinterpret_cast<char *>(&codeKey), 4);
            std::cout << codeKey << ", ";
            inFile.read(reinterpret_cast<char *>(&rowDataFile), sizeof(rowDataFile));
            std::cout << rowDataFile << "\n";
            inFile.get();   // read endLine character
        }
        inFile.close();
    }

};

#endif //PROYECTO1_RANDOMFILE_H
