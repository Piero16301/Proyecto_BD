#ifndef PROYECTO1_STATICHASHING_H
#define PROYECTO1_STATICHASHING_H

#define MB 100 // Numero de bloques

#include <utility>
#include <vector>
#include <map>

#include "Parser.h"
#include "Bucket.h"

using namespace std;

class StaticHashing {
    string indexFile;
    string dataHashFile;
    int bucketCount{};
    map<int,long> indexHashing;

    void buildIndex() {
        fstream index;
        index.open(indexFile, ios::out| ios::trunc);
        index.close();
        index.open(indexFile,ios::out|ios::app|ios::binary);
        if (index) {
            for (int i = 0; i < MB; ++i) {
                long address = i * BucketSize;
                index.write((char *)(&i), sizeof(int));           // Write Key
                index.write((char *)(&address), sizeof(long));    // Write bucket address
                indexHashing[i] = address;                        // Load key-address
            }
        }
        index.close();
    }

    void buildDataFile() {
        fstream dataHash;
        dataHash.open(dataHashFile, ios::out | ios::trunc);
        dataHash.close();
        Bucket temp_bucket;
        dataHash.open(dataHashFile, ios::out | ios::app | ios::binary);
        for (int i = 0; i < MB; ++i) {
            temp_bucket.address = i * BucketSize;
            dataHash << temp_bucket;
        }
        bucketCount = MB;
        dataHash.close();
    }

    void getInitialData(const string& data) {
        fstream file;
        Record temp;
        file.open(data, ios::in | ios::binary);
        while (file >> temp)
            this->insert(temp);
        file.close();
    }

    static Bucket getBucket(long address) {
        Bucket temp;
        fstream file;
        file.open(HASH_FILE_NAME, ios::in | ios::binary);
        file.seekg(address);
        file >> temp;
        file.close();
        return temp;
    }

    static int hashingFunction(int key) {
        return key % MB;
    }

public:
    StaticHashing() = default;

    explicit StaticHashing(const string& data) {
        this->dataHashFile = HASH_FILE_NAME;
        this->indexFile = "../indexHashing.dat";
        buildIndex();
        buildDataFile();
        getInitialData(data);
    }

    void insert(Record record) {
        int index = hashingFunction(record.getCode());
        long address = indexHashing.at(index);
        Bucket bucket = getBucket(address);
        cout << endl << record.getCode() << " pertenece al bucket " <<
             address / BucketSize << endl;

        if (bucket.count >= FB) {
            cout << "bucket " << address / BucketSize << " esta lleno\n";
            cout << "recorrido de overflow: " << address / BucketSize;
            while (bucket.next_overflow > 0) {
                address = bucket.next_overflow;
                bucket = getBucket(address);
                cout  << " -> " << address / BucketSize;
            } cout << endl;
        }

        if (bucket.count == FB) {
            cout << "bucket " << address / BucketSize << " ya estaba lleno, se crea overflow\n";
            bucket.createOverflow(bucketCount);
            bucketCount++;
            address = bucket.next_overflow;
            bucket = getBucket(address);
        }

        bucket.insertRecord(record);
        cout << "Se inserto registro " << record.getCode() <<
             " en bucket " << address / BucketSize << endl;

    }

    void search(int code) {
        cout << "\n*** Search method ***\n";
        auto t1 = std::chrono::high_resolution_clock::now();
        int index = hashingFunction(code);
        long address = indexHashing.at(index);
        Bucket temp = getBucket(address);
        Record record;

        bool found = temp.findRecord(code, record);
        while (!found) {
            if (temp.next_overflow > 0)
                temp = getBucket(temp.next_overflow);
            else break;
            found = temp.findRecord(code, record);
        }
        cout << "Codigo " << code;
        if (found) {
            cout << " encontrado\n";
            record.showData();
        }
        else cout << " no encontrado\n";

        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        std::cout << "\nDuration: " << duration << " microseconds\n";

        //Heap file comparison
        HeapFile heapFile;
        heapFile.search(code);
    }

    void showData() const {
        cout << endl;
        for (int i = 0; i < 110 ; ++i) cout << "-"; cout << "\n";
        printf("| %-8s | %-25s | %-25s | %-30s | %-5s |\n", "  Codigo", "Nombre", "Apellido", "Carrera", "Bucket");
        for (int i = 0; i < 110 ; ++i) cout << "-"; cout << "\n";
        for (int i = 0; i < bucketCount; ++i) {
            Bucket bucket = getBucket(i * BucketSize);
            bucket.showRecords();
        }
        for (int i = 0; i < 110 ; ++i) cout << "-"; cout << "\n";
    }

};

#endif //PROYECTO1_STATICHASHING_H
