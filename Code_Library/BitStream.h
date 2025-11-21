
#ifndef MILESTONE_2_ADS_BITSTREAM_H
#define MILESTONE_2_ADS_BITSTREAM_H

#include <fstream>

/*
  BitStream class
  Handles reading and writing bits to a file instead of full bytes.
  Used for compression (like Huffman encoding).
*/
class BitStream {
private:
    unsigned char byteHolder;    // holds up to 8 bits before writing/reading
    int bitPosition;             // current bit position inside the byte (0–7)
    std::fstream* file;          // pointer to file stream
    bool writingMode;            // true for write mode, false for read mode

public:
    BitStream(std::fstream* fileStream, bool mode);  // constructor
    ~BitStream();                                    // destructor

    // Writing functions
    void writeBit(bool bitValue);                    // write a single bit
    void writeByte(unsigned char value);             // write a full byte (8 bits)
    void pushRemainingBits();                        // write leftover bits if any

    // Reading functions
    bool readBit();                                  // read a single bit
    unsigned char readByte();                        // read a full byte (8 bits)
    bool hasMoreBits();                              // check if bits are left to read

    // Utility
    void resetStream();                              // reset buffer and bit position
};

#endif //MILESTONE_2_ADS_BITSTREAM_H