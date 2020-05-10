#ifndef PROYECTO1_PARSER_H
#define PROYECTO1_PARSER_H

#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

struct Student {
    int Code;
    char Name[15];
    char Surname[20];
    char Career[15];
} Student;

class Parser {
private:
    string csvFile;

public:
    explicit Parser(string file) : csvFile{std::move(file)} {}

    void csvToBinaryFile() {
        ifstream file(csvFile);
        if (file.is_open()) {
            string fields[4], row;
            getline(file, row); //Skip field titles
            FILE *binaryFile;
            binaryFile = fopen("../Students", "w");
            while (!file.eof()) {
                getline(file, row);
                istringstream istringstream1(row);
                unsigned int counter = 0;
                while (getline(istringstream1, row, ',')) {
                    fields[counter] = row;
                    counter++;
                }
                struct Student student{};
                student.Code = stoi(fields[0]);
                strcpy(student.Name, fields[1].c_str());
                strcpy(student.Surname, fields[2].c_str());
                strcpy(student.Career, fields[3].c_str());
                fwrite(&student, sizeof(Student), 1, binaryFile);
            }
            fclose(binaryFile);
        }
    }
};

#endif //PROYECTO1_PARSER_H