#include <iostream>

#include "Parser.h"
#include "RandomFile.h"
using namespace std;

int main() {
    do {
        int menu;
        cout << "======== SELECT AN OPTION ========\n1. RANDOM FILE\n2. STATIC HASH\n3. EXIT\n";
        cout << "Insert an option: ";
        cin >> menu;
        Parser parser("../students.csv", "../Students");
        parser.csvToBinaryFile();
        RandomFile randomFile("../Students");
        if (menu == 1) {
            int option;
            bool back = false;
            //Load binary file to Random File structure
            do {
                cout << "\n======== SELECT AN OPTION ========\n";
                cout << "\n1. INSERT RECORD\n2. SEARCH RECORD\n3. BACK\n4. EXIT\n";
                cout << "\nInsert an option: ";
                cin >> option;
                switch (option) {
                    case 1: {
                        //Insert a new record to current Random File structure
                        Record record{};
                        ifstream temp;
                        temp.open("../Students", std::ios::in | std::ios::binary);
                        while (temp >> record) {
                            record.showData();
                        }
                        break;
                    }
                    case 2: {
                        //Search a a record in Random File
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
        else if (menu == 2) {
            int option;
            bool back = false;
            //Load binary file to Static Hash structure
            do {
                cout << "\n======== SELECT AN OPTION ========\n";
                cout << "\n1. INSERT RECORD\n2. SEARCH RECORD\n3. BACK\n4. EXIT\n";
                cout << "\nInsert an option: ";
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
