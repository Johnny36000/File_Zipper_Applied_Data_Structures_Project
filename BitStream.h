#ifndef FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_BITSTREAM_H
#define FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_BITSTREAM_H
#include <fstream>

/**
 * BitStream class - Handles reading and writing individual bits
 * Essential for Huffman compression/decompression
 */
class BitStream {
private:
    unsigned char buffer;
    int bitPosition;
    std::fstream* file;
    bool writeMode;

public:
    BitStream(std::fstream* f, bool isWrite);
    ~BitStream();

    // Write operations
    void writeBit(bool bit);
    void writeByte(unsigned char byte);
    void flush();

    // Read operations
    bool readBit();
    unsigned char readByte();
    bool hasMoreBits();

    // Utility
    void reset();
};

#endif //FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_BITSTREAM_H