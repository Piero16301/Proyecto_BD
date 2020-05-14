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

    void buildDataFile(const string& data) {
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

    static int hashingFunction(int key) {
        return key % MB;
    }

public:
    StaticHashing() = default;

    explicit StaticHashing(const string& data) {
        this->dataHashFile = HASH_FILE_NAME;
        this->indexFile = "../indexHashing.dat";
        buildIndex();
        buildDataFile(data);
    }

    void insert(Record record) {
        int index = hashingFunction(record.getCode());
        long address = indexHashing.at(index);
        Bucket bucket = getBucket(address);
        if (bucket.count >= FB) {
            if (bucket.next_overflow == -1) {
                createOvfBucket(bucket, bucketCount);
            }
            while (bucket.next_overflow > 0) {
                address = bucket.next_overflow;
                bucket = getBucket(bucket.next_overflow);
            }
        } insertIntoBucket(record, bucket, address);
        //bucket.insertRecord(record, address);
    }



};

#endif //PROYECTO1_STATICHASHING_H
