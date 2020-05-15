#ifndef PROYECTO1_HEAPFILE_H
#define PROYECTO1_HEAPFILE_H

#include <string>
#include <utility>
#include <fstream>
#include <chrono>

#include "Record.h"

class HeapFile {
private:
    std::string dataFile = "../Students.dat";

public:
    HeapFile() = default;

    void search(int code) {
        Record record{};
        ifstream data_File;
        auto t1 = std::chrono::high_resolution_clock::now();
        data_File.open(dataFile, std::ios::in | std::ios::binary);
        while (data_File >> record) {
            if (record.getCode() == code) {
                break;
            }
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        std::cout << "\nHeap file: " << duration << " microseconds\n";
    }
};

#endif //PROYECTO1_HEAPFILE_H