#ifndef PROYECTO1_BUCKET_H
#define PROYECTO1_BUCKET_H

#define FB 10            // Factor de bloque
#define HASH_FILE_NAME "../dataHashing.dat"

#include "Record.h"

struct Bucket {
    Record records[FB];
    long address = -1;
    long next_overflow = -1;
    bool taken[FB]{};
    int count{};
    bool findRecord(int code, Record& record);
    void insertRecord(Record record);
    void createOverflow(int totalBuckets);
    void showRecords();
};

const int BucketSize = (int)sizeof(Bucket);

ostream& operator << (ostream& stream, Bucket& bucket) {
    stream.write((char*)&bucket, BucketSize);
    return stream;
}

istream& operator >> (istream& stream, Bucket& bucket) {
    stream.read((char*)&bucket, BucketSize);
    return stream;
}

bool Bucket::findRecord(int code, Record& record) {
    if (this->count > 0)
        for (int i = 0; i < FB; ++i) {
            if (taken[i] && records[i].getCode() == code) {
                record = records[i];
                return true;
            }
        }
    return false;
}

void Bucket::insertRecord(Record record) {
    fstream file;
    int i;
    for (i = 0; i < FB; ++i) {
        if (!this->taken[i]) {
            this->records[i] = record;
            this->taken[i] = true;
            this->count++;
            file.open(HASH_FILE_NAME, ios::in|ios::out|ios::binary);
            file.seekg(this->address);
            file << *this;
            file.close();
            break;
        }
    } if (i >= FB) cout << "Error de insercion\n";
}

void Bucket::createOverflow(int totalBuckets) {
    Bucket ovf_bucket;
    this->next_overflow = ovf_bucket.address = totalBuckets * BucketSize;
    fstream file;
    file.open(HASH_FILE_NAME, ios::out|ios::app|ios::binary);
    file << ovf_bucket;
    file.close();
    file.open(HASH_FILE_NAME, ios::in|ios::out|ios::binary);
    file.seekg(this->address);
    file << *this;
    file.close();
}

void Bucket::showRecords() {
    if (count == 0) return;
    for (int i = 0; i < FB; ++i) {
        if (taken[i]) {
            records[i].showRecord(address/BucketSize);
        }
    }
}

#endif //PROYECTO1_BUCKET_H