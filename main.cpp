#include "HuffmanZipper.h"
#include "HuffmanNode.h"
#include "MiniHeap.h"
#include "BitStream.h"
#include "HashMap.h"
#include "DynamicArray.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>


using namespace std;

int main() {
    int choice;
    string inputFile, outputFile;

    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear newline

        switch (choice) {
        case 1:
            cout << "Enter input file name: ";
            getline(cin, inputFile);
            cout << "Enter output file name: ";
            getline(cin, outputFile);
            compressFile(inputFile, outputFile);
            break;

        case 2:
            cout << "Enter compressed file name: ";
            getline(cin, inputFile);
            cout << "Enter output file name: ";
            getline(cin, outputFile);
            decompressFile(inputFile, outputFile);
            break;

        case 3:
            cout << "Exiting program. Goodbye!" << endl;
            return 0;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}