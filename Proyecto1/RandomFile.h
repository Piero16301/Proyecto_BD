#ifndef PROYECTO1_RANDOMFILE_H
#define PROYECTO1_RANDOMFILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <map>
#include <utility>
#include <chrono>

#include "Parser.h"
#include "Record.h"
#include "HeapFile.h"

class RandomFile {
private:
    std::string indexFile;
    std::string dataFile;
    int rowsIndexFile{};
    int sizeRecord{};
    int numDelRecords{};
    std::map<int, pair<int, bool>> newIndexRandomMap;

    void updateIndexFile(int code, int row, bool del){
        std::cout << "Update Index File (disk)\n";
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

    void updateIndexFileDeleteOperation(int code){
        std::cout << "Update Index File (disk) Delete Operation\n";
        std::fstream outIndexFile;
        outIndexFile.open(indexFile, std::ios::out | std::ios::in | std::ios::binary);
        if(outIndexFile.is_open()){
            int codeKey;
            int rowDataFile;
            bool del;
            long long int pos;
            for(int i = 0; i < rowsIndexFile; i++){
                outIndexFile.read(reinterpret_cast<char *>(&codeKey), 4);
                if(codeKey == code){
                    del = true;
                    pos = outIndexFile.tellg();
                    pos += 4;
                    outIndexFile.seekp(pos, std::ios::beg);
                    outIndexFile.write((char *)(&del), sizeof(del));
                }
                outIndexFile.read(reinterpret_cast<char *>(&rowDataFile), sizeof(rowDataFile));
                outIndexFile.read(reinterpret_cast<char *>(&del), sizeof(del));
                outIndexFile.get();   // read endLine character
            }
        }
        else{
            std::cout << "Error opening indexFile\n";
        }
        outIndexFile.close();
    }

    void updateIndexFileReUseDelSpace(int deletedCode, int newCode, int row){
        std::cout << "Update Index File (disk) ReUse Deleted Space\n";
        std::cout << "Searching deleted record: " << deletedCode << ", to insert new Key record: " << newCode << '\n';
        std::fstream outIndexFile;
        outIndexFile.open(indexFile, std::ios::out | std::ios::in | std::ios::binary);
        if(outIndexFile.is_open()){
            int codeKey, rowDataFile;
            long long int pos;
            bool del;
            for(int i = 0; i < rowsIndexFile; i++){
                outIndexFile.read(reinterpret_cast<char *>(&codeKey), sizeof(codeKey));
                if(codeKey == deletedCode){
                    bool delStatus = false;
                    std::cout << "Updating deleted record in index File\n";
                    pos = outIndexFile.tellg();
                    pos -= 4;
                    outIndexFile.seekp(pos, std::ios::beg);
                    outIndexFile.write((char *)(&newCode), sizeof(newCode));
                    outIndexFile.write((char *)(&row), sizeof(row));
                    outIndexFile.write((char *)(&del), sizeof(delStatus));
                    break;
                }
                outIndexFile.read(reinterpret_cast<char *>(&rowDataFile), sizeof(rowDataFile));
                outIndexFile.read(reinterpret_cast<char *>(&del), sizeof(del));
                outIndexFile.get();   // read endLine character
            }
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

    int existDeletedRecords(){
        for(auto element : newIndexRandomMap){
            if(element.second.second){
                return element.first;   // return keyCode
            }
        }
        return -1;
    }

public:
    // Default Constructor
    RandomFile() = default;

    // Custom Constructor
    explicit RandomFile(std::string _dataFile){
        dataFile = std::move(_dataFile);
        indexFile = "../indexRandom.dat";
        rowsIndexFile = 0;
        sizeRecord = 85;
        numDelRecords = 0;
        generateIndex();        // generate index in disk
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
        auto t1 = std::chrono::high_resolution_clock::now();
        auto itrResult = newIndexRandomMap.find(code);
        if(itrResult != newIndexRandomMap.end() && !itrResult->second.second){
            std::cout << "Code Found! row value is: " << itrResult->second.first << "\n";
            readRecord(itrResult->second.first);
        }
        else{
            std::cout << "Code '" << code << "' does not exist\n";
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        std::cout << "\nDuration: " << duration << " microseconds\n";

        //Heap file comparison
        HeapFile heapFile;
        heapFile.search(code);
    }

    void insert(Record record) {
        std::cout << "\n*** Insert method ***\n";
        std::cout << "Inserting record code: " << record.getCode() << '\n';
        auto t1 = std::chrono::high_resolution_clock::now();
        std::fstream outFile;
        // Check if exist deleted records
        int deletedKeyRecord = existDeletedRecords();
        if(deletedKeyRecord >= 0){
            // Update Index Map (Ram)
            auto itrResult = newIndexRandomMap.find(deletedKeyRecord);
            int rowNumber = itrResult->second.first;
            newIndexRandomMap.erase(itrResult); // Delete deleted record from Map
            // Create new entry in indexMap
            bool del = false;
            pair<int, bool> rowReuseDelPair(rowNumber, del);
            newIndexRandomMap.insert(std::make_pair(record.getCode(), rowReuseDelPair));
            // Update Index File (disk)
            updateIndexFileReUseDelSpace(deletedKeyRecord, record.getCode(), rowNumber);
            showIndexRandomFile();
            // Insert record in DataFile
            outFile.open(dataFile, std::ios::out | std::ios::in | std::ios::binary);
            outFile.seekp(sizeRecord * rowNumber, std::ios::beg);
            outFile << record;
        }
        else{
            // If not exist deleted records. Update data File
            outFile.open(dataFile, std::ios::out |std::ios::app | std::ios::binary);
            outFile << record;
            // Update indexRandomMap (Memory)
            bool del = false;
            pair<int, bool> rowDelPair(rowsIndexFile,del);
            newIndexRandomMap.insert(std::make_pair(record.getCode(), rowDelPair));
            // Update IndexRandomFile (Disk)
            updateIndexFile(record.getCode(), rowsIndexFile, del);
            // Increase total number of Rows
            rowsIndexFile++;
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        std::cout << "\nDuration: " << duration << " microseconds\n";
    }

    void readAllRecords(){
        std::cout << "\n*** Read all records ***\n";
        Record record{};
        ifstream data_File;
        data_File.open(dataFile, std::ios::in | std::ios::binary);
        while (data_File >> record) {
            auto itrResult = newIndexRandomMap.find(record.getCode());
            if(itrResult != newIndexRandomMap.end() && !itrResult->second.second){
                record.showData();
            }
        }
    }

    void deleteRecord(int code){
        std::cout << "\n*** Delete record ***\n";
        auto itrResult = newIndexRandomMap.find(code);
        if(itrResult != newIndexRandomMap.end() && !itrResult->second.second){
            std::cout << "Code Found! row value is: " << itrResult->second.first << "\n";
            std::cout << "Deleting record\n";
            // Update index (Ram)
            itrResult->second.second = true;
            // Update index (disk)
            updateIndexFileDeleteOperation(code);
            numDelRecords++;
            // Validation - Show indexRandom File from Disk
            showIndexRandomFile();
        }
        else{
            std::cout << "Code '" << code << "' does not exist or is already deleted\n";
        }
    }

};

#endif //PROYECTO1_RANDOMFILE_H
