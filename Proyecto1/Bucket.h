#ifndef PROYECTO1_BUCKET_H
#define PROYECTO1_BUCKET_H

#define FB 10            // Factor de bloque
#define HASH_FILE_NAME "../dataHashing.dat"

#include "Record.h"

struct Bucket {
    Record records[FB];
    //int position = -1;
    long next_overflow = -1;
    bool taken[FB]{};
    int count{};
    void insertRecord(Record record, long address);
    void createOverflow(long address, int position);
    void showRecords();
};

ostream& operator << (ostream& stream, Bucket& bucket) {
    stream.write((char*)&bucket, sizeof(Bucket));
    return stream;
}

istream& operator >> (istream& stream, Bucket& bucket) {
    stream.read((char*)&bucket, sizeof(Bucket));
    return stream;
}

void Bucket::insertRecord(Record record, long address) {
    fstream file;
    int i;
    for (i = 0; i < FB; ++i) {
        if (!this->taken[i]) {
            this->records[i] = record;
            this->taken[i] = true;
            this->count++;
            file.open(HASH_FILE_NAME, ios::in|ios::out|ios::binary);
            file.seekg(address);
            file << *this;
            file.close();
            break;
        }
    }
    if (i >= FB) cout << "Error de insercion\n";
}

void Bucket::createOverflow(long address, int position) {
    Bucket ovf_bucket;
    fstream file;
    file.open(HASH_FILE_NAME, ios::in|ios::out|ios::app|ios::binary);
    file << ovf_bucket;
    file.close();
    this->next_overflow = position * (int)sizeof(Bucket);
    file.open(HASH_FILE_NAME, ios::in|ios::out|ios::binary);
    file.seekg(address);
    file << *this;
    file.close();
}

void Bucket::showRecords() {
    if (count == 0) return;
    for (int i = 0; i < FB; ++i) {
        if (taken[i]) {
            records[i].showData();
        }
    }
}

#endif //PROYECTO1_BUCKET_H