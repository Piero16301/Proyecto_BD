#ifndef PROYECTO1_STATICHASHING_H
#define PROYECTO1_STATICHASHING_H

#define MB 6 // Numero de bloques

#include <utility>
#include <vector>
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
            for (int i=0; i<MB; ++i) {
                long address = i*(int)sizeof(Bucket);
                index.write((char *)(&i), sizeof(int));           // Write Key
                index.write((char *)(&address), sizeof(long));    // Write bucket address
                indexHashing[i] = address;                        // Load key-address
            }
        }
        index.close();
        //for (auto x : indexHashing) cout << x.first << " " << x.second << endl;
    }

    void buildDataFile() {
        fstream dataHash;
        dataHash.open(dataHashFile, ios::out | ios::trunc);
        dataHash.close();
        Bucket temp_bucket;
        dataHash.open(dataHashFile, ios::out | ios::app | ios::binary);
        for (int i = 0; i < MB; ++i)
            dataHash << temp_bucket;
        bucketCount = MB;
        //long byte = dataHash.tellg();
        //cout << "File size in: " << byte/sizeof(Bucket) << endl;
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

    Bucket getBucket(long address) {
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
                address/sizeof(Bucket) << endl;

        if (bucket.count >= FB) {
            cout << "bucket " << address/sizeof(Bucket) << " esta lleno\n";
            cout << "recorrido de overflow: " << address/sizeof(Bucket);
            while (bucket.next_overflow > 0) {
                address = bucket.next_overflow;
                bucket = getBucket(bucket.next_overflow);
                cout  << " -> " << address/sizeof(Bucket);
            } cout << endl;
        }

        bucket.insertRecord(record, address);
        cout << "Se inserto registro " << record.getCode() <<
                " en bucket " << address/sizeof(Bucket) << endl;

        if (bucket.count == FB) {
            bucket.createOverflow(address, bucketCount);
            bucketCount++;
            cout << "Bucket lleno, se crea overflow\n";
        }
    }

    void showData() {
        for (int i = 0; i < bucketCount; ++i) {
            cout << endl << "Bucket " << i << endl;
            Bucket bucket = getBucket(i*(int)sizeof(Bucket));
            bucket.showRecords();
        }
    }

};

#endif //PROYECTO1_STATICHASHING_H
