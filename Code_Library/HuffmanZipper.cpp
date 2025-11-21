
#include "HuffmanZipper.h"
#include "MiniHeap.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * Build frequency map using HashMap
 */
void buildFrequencyMap(const string& filename, HashMap& freqMap) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    unsigned char ch;
    while (file.get((char&)ch)) {
        int currentFreq = 0;
        char charKey = (char)ch;  // Explicit cast
        if (freqMap.find(charKey, currentFreq)) {
            freqMap.insert(charKey, currentFreq + 1);
        } else {
            freqMap.insert(charKey, 1);
        }
    }

    file.close();
}

/**
 * Build Huffman tree from HashMap
 */
HuffmanNode* buildHuffmanTree(HashMap& freqMap) {
    MinHeap heap;

    // Iterate through all possible byte values
    for (int i = 0; i < 256; i++) {
        int freq;
        if (freqMap.find((unsigned char)i, freq) && freq > 0) {
            HuffmanNode* node = new HuffmanNode((unsigned char)i, freq);
            heap.insert(node);
        }
    }

    // Handle edge cases
    if (heap.empty()) {
        return nullptr;
    }

    if (heap.size() == 1) {
        HuffmanNode* single = heap.extractMin();
        // Create a dummy right node for symmetry
        HuffmanNode* dummy = new HuffmanNode(0, 0);
        HuffmanNode* root = new HuffmanNode(single->frequency, single, dummy);
        return root;
    }


    // Build tree
    while (heap.size() > 1) {
        HuffmanNode* left = heap.extractMin();
        HuffmanNode* right = heap.extractMin();
        HuffmanNode* parent = new HuffmanNode(
            left->frequency + right->frequency,
            left,
            right
        );
        heap.insert(parent);
    }

    return heap.extractMin();
}

/**
 * Generate Huffman codes for each character
 * Traverse tree and assign binary codes
 */
void generateCodes(HuffmanNode* root, string code, string codes[256]) {
    if (!root) return;

    // Leaf node - store the code
    if (root->isLeaf()) {
        codes[root->data] = code.empty() ? "0" : code; // Handle single character case
        return;
    }

    // Traverse left (add '0') and right (add '1')
    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

/**
 * Serialize Huffman tree to compressed file
 * Write tree structure using pre-order traversal
 * Format: 0 for internal node, 1 followed by byte value for leaf
 */
void serializeTree(HuffmanNode* root, BitStream& bs) {
    if (!root) return;

    if (root->isLeaf()) {
        bs.writeBit(true);  // 1 indicates leaf node
        bs.writeByte(root->data);  // Write the character
    } else {
        bs.writeBit(false); // 0 indicates internal node
        serializeTree(root->left, bs);
        serializeTree(root->right, bs);
    }
}

/**
 * Deserialize Huffman tree from compressed file
 * Reconstruct tree from pre-order traversal
 */
HuffmanNode* deserializeTree(BitStream& bs) {
    bool isLeaf = bs.readBit();

    if (isLeaf) {
        unsigned char ch = bs.readByte();
        return new HuffmanNode(ch, 0);
    } else {
        HuffmanNode* left = deserializeTree(bs);
        HuffmanNode* right = deserializeTree(bs);
        return new HuffmanNode(0, left, right);
    }
}

/**
 * Compress a file using Huffman encoding
 */
void compressFile(const string& inputFile, const string& outputFile) {
    cout << "Compressing " << inputFile << "..." << endl;

    // Step 1: Build frequency map using HashMap
    HashMap freqMap(256);
    buildFrequencyMap(inputFile, freqMap);

    // Step 2: Build Huffman tree
    HuffmanNode* root = buildHuffmanTree(freqMap);
    if (!root) {
        cerr << "Error: Empty file or cannot build tree" << endl;
        return;
    }

    // Step 3: Generate codes (keep array for fast lookup during encoding)
    string codes[256];
    for (int i = 0; i < 256; i++) {
        codes[i] = "";
    }
    generateCodes(root, "", codes);

    // Step 4: Write compressed file
    fstream outFile(outputFile, ios::out | ios::binary);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot create output file" << endl;
        delete root;
        return;
    }

    BitStream bs(&outFile, true); // Write mode

    // Write original file size (for decompression verification)
    ifstream inFile(inputFile, ios::binary);
    inFile.seekg(0, ios::end);
    unsigned int fileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);

    // Write file size as 4 bytes
    for (int i = 3; i >= 0; i--) {
        bs.writeByte((fileSize >> (i * 8)) & 0xFF);
    }

    // Serialize tree structure
    serializeTree(root, bs);

    // Encode file content
    unsigned char ch;
    while (inFile.get((char&)ch)) {
        string code = codes[ch];
        for (char bit : code) {
            bs.writeBit(bit == '1');
        }
    }

    bs.pushRemainingBits(); // Flush remaining bits

    inFile.close();
    outFile.close();

    // Cleanup
    delete root;

    cout << "Compression complete! Output: " << outputFile << endl;
}

/**
 * Decompress a Huffman-encoded file
 */
void decompressFile(const string& inputFile, const string& outputFile) {
    cout << "Decompressing " << inputFile << "..." << endl;

    fstream inFile(inputFile, ios::in | ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open compressed file" << endl;
        return;
    }

    BitStream bs(&inFile, false); // Read mode

    // Read original file size
    unsigned int fileSize = 0;
    for (int i = 0; i < 4; i++) {
        fileSize = (fileSize << 8) | bs.readByte();
    }

    // Deserialize tree
    HuffmanNode* root = deserializeTree(bs);
    if (!root) {
        cerr << "Error: Cannot reconstruct tree" << endl;
        inFile.close();
        return;
    }

    // Decode content
    ofstream outFile(outputFile, ios::binary);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot create output file" << endl;
        delete root;
        inFile.close();
        return;
    }

    HuffmanNode* current = root;
    unsigned int bytesWritten = 0;

    while (bytesWritten < fileSize && bs.hasMoreBits()) {
        bool bit = bs.readBit();

        // Traverse tree with null check
        if (bit) {
            if (current->right) {
                current = current->right;
            } else {
                cerr << "Error: Invalid tree traversal (right)" << endl;
                break;
            }
        } else {
            if (current->left) {
                current = current->left;
            } else {
                cerr << "Error: Invalid tree traversal (left)" << endl;
                break;
            }
        }

        // Reached leaf node
        if (current && current->isLeaf()) {
            outFile.put(current->data);
            bytesWritten++;
            current = root; // Reset to root for next character
        }
    }


    outFile.close();
    inFile.close();

    // Cleanup
    delete root;

    cout << "Decompression complete! Output: " << outputFile << endl;
}

/**
 * Display menu
 */
void displayMenu() {
    cout << "\n========================================" << endl;
    cout << "    Huffman File Zipper" << endl;
    cout << "========================================" << endl;
    cout << "1. Compress a file" << endl;
    cout << "2. Decompress a file" << endl;
    cout << "3. Exit" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice: ";
}