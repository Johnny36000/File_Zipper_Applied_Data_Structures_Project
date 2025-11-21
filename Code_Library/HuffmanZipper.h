

#ifndef MILESTONE_2_ADS_HUFFMANZIPPER_H
#define MILESTONE_2_ADS_HUFFMANZIPPER_H

#include <string>
#include "HuffmanNode.h"
#include "HashMap.h"
#include "BitStream.h"

using namespace std;

// Function declarations (prototypes)

/**
 * Build frequency map for all bytes in the file
 */
void buildFrequencyMap(const string& filename, HashMap& freqMap);


/**
 * Build Huffman tree from frequency array
 */
HuffmanNode* buildHuffmanTree(HashMap& freqMap);

/**
 * Generate Huffman codes for each character
 * Traverse tree and assign binary codes
 */
void generateCodes(HuffmanNode* root, string code, string codes[256]);

/**
 * Serialize Huffman tree to compressed file
 * Write tree structure using pre-order traversal
 */
void serializeTree(HuffmanNode* root, BitStream& bs);

/**
 * Deserialize Huffman tree from compressed file
 * Reconstruct tree from pre-order traversal
 */
HuffmanNode* deserializeTree(BitStream& bs);

/**
 * Compress a file using Huffman encoding
 */
void compressFile(const string& inputFile, const string& outputFile);

/**
 * Decompress a Huffman-encoded file
 */
void decompressFile(const string& inputFile, const string& outputFile);

/**
 * Display menu
 */
void displayMenu();

#endif //MILESTONE_2_ADS_HUFFMANZIPPER_H