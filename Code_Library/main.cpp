#include <iostream>
#include <fstream>
#include <cstring>
#include "HuffmanNode.h"
#include "MinHeap.h"
#include "HashMap.h"
#include "DynamicArray.h"
#include "BitStream.h"

using namespace std;

/**
 * HuffmanCoder - Main class for compression and decompression
 */
class HuffmanCoder {
private:
    HashMap<unsigned char, unsigned int> frequencyMap;
    HashMap<unsigned char, DynamicArray<bool>*> codeMap;
    HuffmanNode* root;

    // Helper functions
    void buildFrequencyMap(const char* inputFile);
    void buildHuffmanTree();
    void generateCodes(HuffmanNode* node, DynamicArray<bool>* code);
    void writeHeader(fstream& outFile);
    void readHeader(fstream& inFile);
    void encodeFile(const char* inputFile, const char* outputFile);
    void decodeFile(const char* inputFile, const char* outputFile);
    void deleteTree(HuffmanNode* node);
    void reconstructTreeFromHeader();

public:
    HuffmanCoder();
    ~HuffmanCoder();

    void compress(const char* inputFile, const char* outputFile);
    void decompress(const char* inputFile, const char* outputFile);
};

/**
 * Constructor
 */
HuffmanCoder::HuffmanCoder() {
    frequencyMap = HashMap<unsigned char, unsigned int>(256);
    codeMap = HashMap<unsigned char, DynamicArray<bool>*>(256);
    root = nullptr;
}

/**
 * Destructor
 */
HuffmanCoder::~HuffmanCoder() {
    // Clean up code map
    typename HashMap<unsigned char, DynamicArray<bool>*>::Iterator it = codeMap.getIterator();
    while (it.hasNext()) {
        delete it.getValue();
        it.next();
    }

    // Delete tree
    if (root != nullptr) {
        deleteTree(root);
    }
}

/**
 * Delete Huffman tree recursively
 */
void HuffmanCoder::deleteTree(HuffmanNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

/**
 * Build frequency map from input file
 */
void HuffmanCoder::buildFrequencyMap(const char* inputFile) {
    ifstream file(inputFile, ios::binary);

    if (!file.is_open()) {
        cout << "Error: Cannot open input file!" << endl;
        return;
    }

    unsigned char ch;
    while (file.get((char&)ch)) {
        unsigned int freq;
        if (frequencyMap.find(ch, freq)) {
            frequencyMap.insert(ch, freq + 1);
        } else {
            frequencyMap.insert(ch, 1);
        }
    }

    file.close();
}

/**
 * Build Huffman tree using min-heap
 */
void HuffmanCoder::buildHuffmanTree() {
    MinHeap heap(256);

    // Create leaf nodes for each character
    typename HashMap<unsigned char, unsigned int>::Iterator it = frequencyMap.getIterator();
    while (it.hasNext()) {
        unsigned char ch = it.getKey();
        unsigned int freq = it.getValue();
        HuffmanNode* node = new HuffmanNode(ch, freq);
        heap.insert(node);
        it.next();
    }

    // Build tree by combining nodes
    while (heap.getSize() > 1) {
        HuffmanNode* left = heap.extractMin();
        HuffmanNode* right = heap.extractMin();

        unsigned int combinedFreq = left->frequency + right->frequency;
        HuffmanNode* parent = new HuffmanNode(combinedFreq, left, right);

        heap.insert(parent);
    }

    root = heap.extractMin();
}

/**
 * Generate Huffman codes for each character
 */
void HuffmanCoder::generateCodes(HuffmanNode* node, DynamicArray<bool>* code) {
    if (node == nullptr) return;

    // Leaf node - store the code
    if (node->isLeaf) {
        DynamicArray<bool>* newCode = new DynamicArray<bool>(32);
        for (int i = 0; i < code->getSize(); i++) {
            newCode->pushBack((*code)[i]);
        }
        codeMap.insert(node->data, newCode);
        return;
    }

    // Traverse left (add 0)
    if (node->left != nullptr) {
        code->pushBack(false);
        generateCodes(node->left, code);
        code->popBack();
    }

    // Traverse right (add 1)
    if (node->right != nullptr) {
        code->pushBack(true);
        generateCodes(node->right, code);
        code->popBack();
    }
}

/**
 * Write header with frequency information to compressed file
 */
void HuffmanCoder::writeHeader(fstream& outFile) {
    // Write number of unique characters
    int uniqueChars = frequencyMap.getSize();
    outFile.write((char*)&uniqueChars, sizeof(int));

    // Write each character and its frequency
    typename HashMap<unsigned char, unsigned int>::Iterator it = frequencyMap.getIterator();
    while (it.hasNext()) {
        unsigned char ch = it.getKey();
        unsigned int freq = it.getValue();

        outFile.write((char*)&ch, sizeof(unsigned char));
        outFile.write((char*)&freq, sizeof(unsigned int));

        it.next();
    }
}

/**
 * Read header from compressed file and rebuild frequency map
 */
void HuffmanCoder::readHeader(fstream& inFile) {
    // Clear existing frequency map
    frequencyMap.clear();

    // Read number of unique characters
    int uniqueChars;
    inFile.read((char*)&uniqueChars, sizeof(int));

    // Read each character and its frequency
    for (int i = 0; i < uniqueChars; i++) {
        unsigned char ch;
        unsigned int freq;

        inFile.read((char*)&ch, sizeof(unsigned char));
        inFile.read((char*)&freq, sizeof(unsigned int));

        frequencyMap.insert(ch, freq);
    }
}

/**
 * Compress input file
 */
void HuffmanCoder::compress(const char* inputFile, const char* outputFile) {
    cout << "Starting compression..." << endl;

    // Step 1: Build frequency map
    cout << "Building frequency map..." << endl;
    buildFrequencyMap(inputFile);

    if (frequencyMap.isEmpty()) {
        cout << "Error: Input file is empty!" << endl;
        return;
    }

    // Step 2: Build Huffman tree
    cout << "Building Huffman tree..." << endl;
    buildHuffmanTree();

    // Step 3: Generate codes
    cout << "Generating Huffman codes..." << endl;
    DynamicArray<bool> tempCode(32);
    generateCodes(root, &tempCode);

    // Step 4: Write compressed file
    cout << "Writing compressed file..." << endl;
    encodeFile(inputFile, outputFile);

    cout << "Compression completed successfully!" << endl;
}

/**
 * Encode and write compressed file
 */
void HuffmanCoder::encodeFile(const char* inputFile, const char* outputFile) {
    fstream outFile(outputFile, ios::binary | ios::out);

    if (!outFile.is_open()) {
        cout << "Error: Cannot create output file!" << endl;
        return;
    }

    // Write header
    writeHeader(outFile);

    // Create bit stream
    BitStream bitStream(&outFile, true);

    // Read input file and write encoded bits
    ifstream inFile(inputFile, ios::binary);
    unsigned char ch;

    while (inFile.get((char&)ch)) {
        DynamicArray<bool>* code;
        if (codeMap.find(ch, code)) {
            for (int i = 0; i < code->getSize(); i++) {
                bitStream.writeBit((*code)[i]);
            }
        }
    }

    bitStream.flush();

    inFile.close();
    outFile.close();
}

/**
 * Decompress compressed file
 */
void HuffmanCoder::decompress(const char* inputFile, const char* outputFile) {
    cout << "Starting decompression..." << endl;

    fstream inFile(inputFile, ios::binary | ios::in);

    if (!inFile.is_open()) {
        cout << "Error: Cannot open compressed file!" << endl;
        return;
    }

    // Step 1: Read header and rebuild frequency map
    cout << "Reading header..." << endl;
    readHeader(inFile);

    // Step 2: Rebuild Huffman tree
    cout << "Rebuilding Huffman tree..." << endl;
    buildHuffmanTree();

    // Step 3: Decode file
    cout << "Decoding file..." << endl;
    decodeFile(inputFile, outputFile);

    inFile.close();

    cout << "Decompression completed successfully!" << endl;
}

/**
 * Decode compressed file using Huffman tree
 */
void HuffmanCoder::decodeFile(const char* inputFile, const char* outputFile) {
    fstream inFile(inputFile, ios::binary | ios::in);
    ofstream outFile(outputFile, ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Error: Cannot open files!" << endl;
        return;
    }

    // Skip header
    readHeader(inFile);

    // Calculate total characters to decode
    unsigned int totalChars = 0;
    typename HashMap<unsigned char, unsigned int>::Iterator it = frequencyMap.getIterator();
    while (it.hasNext()) {
        totalChars += it.getValue();
        it.next();
    }

    // Decode using tree traversal
    BitStream bitStream(&inFile, false);
    HuffmanNode* current = root;
    unsigned int decodedChars = 0;

    while (decodedChars < totalChars && bitStream.hasMoreBits()) {
        bool bit = bitStream.readBit();

        // Traverse tree
        if (bit) {
            current = current->right;
        } else {
            current = current->left;
        }

        // Check if leaf node
        if (current != nullptr && current->isLeaf) {
            outFile.put(current->data);
            decodedChars++;
            current = root; // Reset to root for next character
        }
    }

    inFile.close();
    outFile.close();
}

/**
 * Display menu
 */
void displayMenu() {
    cout << "\n===== Huffman File Compression =====\n";
    cout << "1. Compress a file\n";
    cout << "2. Decompress a file\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

/**
 * Main function
 */
int main() {
    HuffmanCoder coder;
    int choice;
    char inputFile[256], outputFile[256];

    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear newline

        switch (choice) {
            case 1:
                cout << "Enter input file path: ";
                cin.getline(inputFile, 256);

                cout << "Enter output file path (compressed): ";
                cin.getline(outputFile, 256);

                coder.compress(inputFile, outputFile);
                break;

            case 2:
                cout << "Enter compressed file path: ";
                cin.getline(inputFile, 256);

                cout << "Enter output file path (decompressed): ";
                cin.getline(outputFile, 256);

                coder.decompress(inputFile, outputFile);
                break;

            case 3:
                cout << "Exiting program. Goodbye!" << endl;
                return 0;

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}
