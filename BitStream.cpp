#include "BitStream.h"

/*
 * Constructor
 * Initializes the bit stream with the given file and mode (read/write).
 */
BitStream::BitStream(std::fstream* fileStream, bool mode) {
    file = fileStream;
    writingMode = mode;
    byteHolder = 0;
    bitPosition = 0;
}

/*
 * Destructor
 * If in write mode and bits remain, write them before closing.
 */
BitStream::~BitStream() {
    if (writingMode && bitPosition > 0) {
        pushRemainingBits();
    }
}

/*
 * writeBit:
 * Writes a single bit (0 or 1) into the byteHolder.
 * When 8 bits are collected, writes the byte to the file.
 */
void BitStream::writeBit(bool bitValue) {
    if (bitValue) {
        byteHolder = byteHolder | (1 << (7 - bitPosition));  // set bit at correct position
    }

    bitPosition++;

    if (bitPosition == 8) {
        file->put(byteHolder);
        byteHolder = 0;
        bitPosition = 0;
    }
}

/*
 * writeByte:
 * Writes 8 bits (a full byte) one by one.
 */
void BitStream::writeByte(unsigned char value) {
    for (int i = 7; i >= 0; i--) {
        bool bitValue = ((value >> i) % 2);
        writeBit(bitValue);

    }
}

/*
 * pushRemainingBits:
 * Writes any leftover bits (less than 8) to the file.
 */
void BitStream::pushRemainingBits() {
    if (bitPosition > 0) {
        file->put(byteHolder);
        byteHolder = 0;
        bitPosition = 0;
    }
}

/*
 * readBit:
 * Reads a single bit from the file.
 * Loads a new byte when the current one is done.
 */
bool BitStream::readBit() {
    if (bitPosition == 0) {
        byteHolder = file->get();  // load new byte
    }

    bool bit = (byteHolder >> (7 - bitPosition)) & 1;
    bitPosition++;

    if (bitPosition == 8) {
        bitPosition = 0;
    }

    return bit;
}

/*
 * readByte:
 * Reads 8 bits (1 byte) by calling readBit repeatedly.
 */
unsigned char BitStream::readByte() {
    unsigned char value = 0;
    for (int i = 0; i < 8; i++) {
        value = (value << 1) | readBit();
    }
    return value;
}

/*
 * hasMoreBits:
 * Checks if the file still has bits left to read.
 */
bool BitStream::hasMoreBits() {
    return !file->eof();
}

/*
 * resetStream:
 * Resets the buffer and bit position.
 */
void BitStream::resetStream() {
    bitPosition = 0;
    byteHolder = 0;
}
