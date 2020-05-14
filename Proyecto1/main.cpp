#include <iostream>

#include "Parser.h"
#include "RandomFile.h"
using namespace std;

int main() {
    do {
        int menu;
        cout << "======== SELECT AN OPTION ========\n1. RANDOM FILE\n2. STATIC HASH\n3. EXIT\n";
        cout << "Select an option: ";
        cin >> menu;
        Parser parser("../1000students.csv", "../Students.dat");
        parser.csvToBinaryFile();
        RandomFile randomFile("../Students.dat");
        if (menu == 1) {
            // RANDOM FILE
            int option;
            bool back = false;
            //Load binary file to Random File structure
            do {
                cout << "\n======== RANDOM FILE - SELECT AN OPTION ========\n";
                cout << "\n1. READ ALL RECORDS\n2. INSERT RECORD\n3. SEARCH RECORD\n4. DELETE RECORD\n5. BACK\n6. EXIT\n";
                cout << "\nSelect an option: ";
                cin >> option;
                switch (option) {
                    case 1: {
                        //READ ALL RECORDS
                        randomFile.readAllRecords();
                        break;
                    }
                    case 2: {
                        //INSERT NEW RECORD
                        cout << "INSERT A NEW RECORD\n";
                        int code;
                        string Name,Surname,Career;
                        cout << "Code: ";
                        cin >> code;
                        cout << "Name: ";
                        cin >> Name;
                        cout << "Surname: ";
                        cin >> Surname;
                        cout << "Career: ";
                        cin >> Career;
                        Record record(code, Name.c_str(), Surname.c_str(), Career.c_str());
                        randomFile.insert(record);
                        break;
                    }
                    case 3: {
                        //SEARCH A RECORD
                        cout << "\nWrite Code to search: ";
                        int code;
                        cin >> code;
                        randomFile.search(code);
                        break;
                    }
                    case 4: {
                        //DELETE RECORD
                        int code;
                        cout << "\nWrite Code to Delete: ";
                        cin >> code;
                        randomFile.deleteRecord(code);
                        break;
                    }
                    case 5: {
                        // BACK
                        back = true;
                        break;
                    }
                    case 6: {
                        // EXIT
                        exit(0);
                    }
                    default: {
                        cout << "Invalid option, try again.\n";
                        break;
                    }
                }
                if (back) break;
            } while (true);
        }
        else if (menu == 2) {
            int option;
            bool back = false;
            //Load binary file to Static Hash structure
            do {
                cout << "\n======== SELECT AN OPTION ========\n";
                cout << "\n1. INSERT RECORD\n2. SEARCH RECORD\n3. BACK\n4. EXIT\n";
                cout << "\nSelect an option: ";
                cin >> option;
                switch (option) {
                    case 1: {
                        //Insert a new record to current Static Hash structure
                        break;
                    }
                    case 2: {
                        //Search a a record in Static Hash
                        break;
                    }
                    case 3: {
                        back = true;
                        break;
                    }
                    case 4: {
                        exit(0);
                    }
                    default: {
                        cout << "Invalid option, try again.\n";
                        break;
                    }
                }
                if (back) break;
            } while (true);
        }
        else if (menu == 3) {
            exit(0);
        } else {
            cout << "Opcion invalida, intenta nuevamente.\n";
        }
    } while (true);
}
