#include "BitStream.h"

/**
 * Constructor
 */
BitStream::BitStream(std::fstream* f, bool isWrite) {
    file = f;
    writeMode = isWrite;
    buffer = 0;
    bitPosition = 0;
}

/**
 * Destructor - flush remaining bits if in write mode
 */
BitStream::~BitStream() {
    if (writeMode && bitPosition > 0) {
        flush();
    }
}

/**
 * Write a single bit
 */
void BitStream::writeBit(bool bit) {
    if (bit) {
        buffer |= (1 << (7 - bitPosition));
    }
    
    bitPosition++;
    
    if (bitPosition == 8) {
        file->put(buffer);
        buffer = 0;
        bitPosition = 0;
    }
}

/**
 * Write a byte
 */
void BitStream::writeByte(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        writeBit((byte >> i) & 1);
    }
}

/**
 * Flush remaining bits (pad with zeros)
 */
void BitStream::flush() {
    if (bitPosition > 0) {
        file->put(buffer);
        buffer = 0;
        bitPosition = 0;
    }
}

/**
 * Read a single bit
 */
bool BitStream::readBit() {
    if (bitPosition == 0) {
        buffer = file->get();
    }
    
    bool bit = (buffer >> (7 - bitPosition)) & 1;
    bitPosition++;
    
    if (bitPosition == 8) {
        bitPosition = 0;
    }
    
    return bit;
}

/**
 * Read a byte
 */
unsigned char BitStream::readByte() {
    unsigned char byte = 0;
    for (int i = 0; i < 8; i++) {
        byte = (byte << 1) | readBit();
    }
    return byte;
}

/**
 * Check if more bits available
 */
bool BitStream::hasMoreBits() {
    return !file->eof();
}

/**
 * Reset bit position
 */
void BitStream::reset() {
    bitPosition = 0;
    buffer = 0;
}