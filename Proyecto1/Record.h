#ifndef PROYECTO1_RECORD_H
#define PROYECTO1_RECORD_H

#include <iostream>
#include <cstring>

using namespace std;

class Record {
private:
    int Code{};
    char Name[25]{};
    char Surname[25]{};
    char Career[30]{};

    friend std::istream& operator >>(std::istream & stream, Record & record);
    friend std::ostream& operator <<(std::ostream & stream, Record & record);

public:
    Record() = default;

    Record(int code, const char *name, const char *surname, const char *career) {
        this->Code = code;
        strcpy(this->Name, name);
        strcpy(this->Surname, surname);
        strcpy(this->Career, career);
    }

    void showData() {
        cout << endl << "Code: " << this->Code << endl;
        cout << "Name: " << this->Name << endl;
        cout << "Surname: " << this->Surname << endl;
        cout << "Career: " << this->Career << endl;
    }

    void showRecord() {
        printf("| %*d |", 8, this->Code);
        printf(" %-25s |", this->Name);
        printf(" %-25s |", this->Surname);
        printf(" %-30s |\n", this->Career);
    }

    int getCode() const {
        return this->Code;
    }

};

std::ostream & operator << (std::ostream & stream, Record & record) {
    stream.write((char *)(&record.Code), sizeof(record.Code));
    stream.write(record.Name, 25);
    stream.write(record.Surname, 25);
    stream.write(record.Career, 30);
    stream << '\n';
    stream << std::flush;
    return stream;
}

std::istream & operator >> (std::istream & stream, Record & record) {
    stream.read(reinterpret_cast<char *>(&record.Code), sizeof(record.Code));
    stream.read(record.Name, 25);
    stream.read(record.Surname, 25);
    stream.read(record.Career, 30);
    stream.get();
    return stream;
}

#endif //PROYECTO1_RECORD_H
