#ifndef PROYECTO1_RANDOMFILE_H
#define PROYECTO1_RANDOMFILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include "Parser.h"
#include "Record.h"

class RandomFile {
private:
    std::string indexFile;
    std::string dataFile;
    int rowsIndexFile;
    int sizeRecord;
    std::map<int, int> indexRandomMap;
    std::map<int, pair<int, bool>> newIndexRandomMap;

    void updateIndexFile(int code, int row, bool del){
        std::cout << code << "," << row << "," << del << "\n";
        std::fstream outIndexFile;
        outIndexFile.open(indexFile, std::ios::out |std::ios::app | std::ios::binary);
        if(outIndexFile.is_open()){
            outIndexFile.write((char *)(&code), sizeof(code));       // Write Key
            outIndexFile.write((char *)(&row), sizeof(row));    // Write Row number
            outIndexFile.write((char *)(&del), sizeof(del));    // Write Del status
            outIndexFile << "\n";
            outIndexFile << std::flush;
        }
        else{
            std::cout << "Error opening indexFile\n";
        }
        outIndexFile.close();
    }

    void generateIndex() {
        std::cout << "\n** Generate Index File **\n";
        std::ofstream outIndexFile;
        outIndexFile.open(indexFile,std::ios::out | std::ios::trunc | std::ios::binary); // trunc 'discard existing content'
        outIndexFile.close();
        std::ifstream inFile;
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        Record record;
        int row=0;
        bool del = false;
        while(inFile >> record) {
            updateIndexFile(record.getCode(), row, del);
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
        bool del;
        for(int i = 0; i < rowsIndexFile; i++){
            inFile.read(reinterpret_cast<char *>(&keyCode), sizeof(rowDataFile));
            inFile.read(reinterpret_cast<char *>(&rowDataFile), sizeof(rowDataFile));
            inFile.read(reinterpret_cast<char *>(&del), sizeof(del));
            std::cout << keyCode << ", " << rowDataFile << ", " << del << '\n';
            //indexRandomMap.insert({keyCode, rowDataFile});
            pair<int, bool> rowDelPair(rowDataFile,del);
            newIndexRandomMap.insert(std::make_pair(keyCode, rowDelPair));
            inFile.get();   // read endLine character
        }
        inFile.close();
    }

    void readRecord(int row){
        std::cout << "Reading record from data File in disk\n";
        Record record;
        std::ifstream inFile;
        inFile.open(dataFile, std::ios::in | std::ios::binary);
        inFile.seekg(sizeRecord * row, std::ios::beg);
        inFile >> record;
        record.showData();
        inFile.close();
    }

public:
    // Default Constructor
    RandomFile() = default;

    // Custom Constructor
    RandomFile(std::string _dataFile){
        dataFile = _dataFile;
        indexFile = "../indexRandom.dat";
        rowsIndexFile = 0;
        sizeRecord = 85;
        generateIndex();
        showIndexRandomFile();  // read from disk
        loadIndexRandomFile();  // load index to Ram

    }

    void showIndexRandomFile() {
        std::cout << "\n** Show Index File from disk **\n";
        std::ifstream inFile;
        inFile.open(indexFile, std::ios::in | std::ios::binary);
        int codeKey;
        int rowDataFile;
        bool delStatus;
        for(int i = 0; i < rowsIndexFile; i++){
            inFile.read(reinterpret_cast<char *>(&codeKey), 4);
            std::cout << codeKey << ", ";
            inFile.read(reinterpret_cast<char *>(&rowDataFile), sizeof(rowDataFile));
            std::cout << rowDataFile << ", ";
            inFile.read(reinterpret_cast<char *>(&delStatus), sizeof(delStatus));
            std::cout << delStatus << "\n";
            inFile.get();   // read endLine character
        }
        inFile.close();
    }

    void search(int code){
        std::cout << "\n*** Search method ***\n";
        std::cout << "searching Code '" << code << "' (from index File in Memory RAM)\n";
        //auto itrResult = indexRandomMap.find(code);
        auto itrResult = newIndexRandomMap.find(code);
        if(itrResult != newIndexRandomMap.end() && !itrResult->second.second){
            std::cout << "Code Found! row value is: " << itrResult->second.first << "\n";
            readRecord(itrResult->second.first);
        }
        else{
            std::cout << "Code '" << code << "' does not exist\n";
        }
    }

    void insert(Record record) {
        std::cout << "\n*** Insert method ***\n";
        std::cout << "Inserting record code: " << record.getCode() << '\n';
        std::fstream outFile;
        outFile.open(dataFile, std::ios::out |std::ios::app | std::ios::binary);
        // Update data File
        outFile << record;
        // Update indexRandomMap (Memory)
        indexRandomMap.insert({record.getCode(), rowsIndexFile});
        // Update IndexRandomFile (Disk)
        bool del = false;
        updateIndexFile(record.getCode(), rowsIndexFile, del);
        // Increase total number of Rows
        rowsIndexFile++;
    }

    void readAllRecords(){
        std::cout << "\n*** Read all records ***\n";
        Record record{};
        ifstream temp;
        temp.open(dataFile, std::ios::in | std::ios::binary);
        while (temp >> record) {
            record.showData();
        }
    }

};

#endif //PROYECTO1_RANDOMFILE_H
