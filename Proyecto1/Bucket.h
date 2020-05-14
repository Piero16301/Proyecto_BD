#ifndef PROYECTO1_BUCKET_H
#define PROYECTO1_BUCKET_H

#define FB 4 // Factor de bloque
#define HASH_FILE_NAME "../dataHashing.dat"

#include "Record.h"

struct Bucket {
    Record records[FB];
    long next_overflow = -1;
    bool taken[FB]{};
    int count{};
};

ostream& operator << (ostream& stream, Bucket& bucket) {
    stream.write((char*)&bucket, sizeof(Bucket));
    return stream;
}

istream& operator >> (istream& stream, Bucket& bucket) {
    stream.read((char*)&bucket, sizeof(Bucket));
    return stream;
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

void insertIntoBucket(Record record, Bucket& temp, long address) {
    fstream file;
    for (int i = 0; i < FB; ++i) {
        if (!temp.taken[i]) {
            temp.records[i] = record;
            temp.taken[i] = true;
            temp.count++;
            file.open(HASH_FILE_NAME, ios::in|ios::out|ios::binary);
            file.seekg(address);
            file << temp;
            file.close();
            //cout << i << " " << temp.taken[i] << " " << temp.count << endl;
            break;
        }
    }
}

void createOvfBucket(Bucket& bucket, int& bucketCount) {
    Bucket ovf_bucket;
    fstream file;
    file.open(HASH_FILE_NAME, ios::in|ios::out|ios::app|ios::binary);
    file << ovf_bucket;
    file.close();
    bucket.next_overflow = bucketCount*(int)sizeof(Bucket);
    bucketCount++;
}

#endif //PROYECTO1_BUCKET_H