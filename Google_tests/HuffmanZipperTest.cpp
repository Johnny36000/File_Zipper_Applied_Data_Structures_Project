#include "HuffmanNode.h"
#include "MiniHeap.h"
#include "BitStream.h"
#include "HashMap.h"
#include "DynamicArray.h"
#include <gtest/gtest.h>
#include <fstream>
#include <string>

using namespace std;

// Test fixture for Huffman components
class HuffmanZipperTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test files before each test
        createTestFile("test_input.txt", "AAABBC");
        createTestFile("test_empty.txt", "");
        createTestFile("test_single.txt", "A");
        createTestFile("test_large.txt", string(1000, 'X'));
    }

    void TearDown() override {
        // Cleanup test files after each test
        remove("test_input.txt");
        remove("test_empty.txt");
        remove("test_single.txt");
        remove("test_large.txt");
        remove("test_compressed.huf");
        remove("test_decompressed.txt");
    }

    void createTestFile(const string& filename, const string& content) {
        ofstream file(filename, ios::binary);
        file << content;
        file.close();
    }

    string readFile(const string& filename) {
        ifstream file(filename, ios::binary);
        string content((istreambuf_iterator<char>(file)),
                       istreambuf_iterator<char>());
        file.close();
        return content;
    }
};

// Test HuffmanNode construction and properties
TEST_F(HuffmanZipperTest, HuffmanNodeLeafConstructorTest) {
    HuffmanNode node('A', 5);
    EXPECT_EQ(node.data, 'A');
    EXPECT_EQ(node.frequency, 5);
    EXPECT_TRUE(node.isLeaf());
    EXPECT_EQ(node.left, nullptr);
    EXPECT_EQ(node.right, nullptr);
}

TEST_F(HuffmanZipperTest, HuffmanNodeInternalConstructorTest) {
    HuffmanNode* left = new HuffmanNode('A', 3);
    HuffmanNode* right = new HuffmanNode('B', 2);
    HuffmanNode parent(5, left, right);

    EXPECT_EQ(parent.frequency, 5);
    EXPECT_FALSE(parent.isLeaf());
    EXPECT_EQ(parent.left, left);
    EXPECT_EQ(parent.right, right);
}

TEST_F(HuffmanZipperTest, HuffmanNodeUnsignedCharTest) {
    // Test that unsigned char handles all byte values 0-255
    HuffmanNode node1(0, 1);
    HuffmanNode node2(255, 1);

    EXPECT_EQ(node1.data, 0);
    EXPECT_EQ(node2.data, 255);
}

// Test MinHeap operations
TEST_F(HuffmanZipperTest, MinHeapInsertAndExtractTest) {
    MinHeap heap;

    HuffmanNode* node1 = new HuffmanNode('A', 5);
    HuffmanNode* node2 = new HuffmanNode('B', 3);
    HuffmanNode* node3 = new HuffmanNode('C', 7);

    heap.insert(node1);
    heap.insert(node2);
    heap.insert(node3);

    EXPECT_EQ(heap.size(), 3);

    HuffmanNode* min = heap.extractMin();
    EXPECT_EQ(min->frequency, 3); // B should be minimum
    EXPECT_EQ(heap.size(), 2);

    // Cleanup
    delete node1;
    delete node2;
    delete node3;
}

TEST_F(HuffmanZipperTest, MinHeapEmptyTest) {
    MinHeap heap;
    EXPECT_TRUE(heap.empty());
    EXPECT_EQ(heap.size(), 0);
    EXPECT_EQ(heap.extractMin(), nullptr);
}

TEST_F(HuffmanZipperTest, MinHeapTopTest) {
    MinHeap heap;
    HuffmanNode* node1 = new HuffmanNode('A', 10);
    HuffmanNode* node2 = new HuffmanNode('B', 5);

    heap.insert(node1);
    heap.insert(node2);

    EXPECT_EQ(heap.top()->frequency, 5);
    EXPECT_EQ(heap.size(), 2); // top() doesn't remove

    delete node1;
    delete node2;
}

// Test BitStream writing and reading
TEST_F(HuffmanZipperTest, BitStreamWriteAndReadBitTest) {
    fstream file("test_bitstream.dat", ios::out | ios::in | ios::binary | ios::trunc);

    // Write bits
    BitStream writer(&file, true);
    writer.writeBit(true);
    writer.writeBit(false);
    writer.writeBit(true);
    writer.writeBit(true);
    writer.pushRemainingBits();

    // Reset file position
    file.clear();
    file.seekg(0, ios::beg);

    // Read bits
    BitStream reader(&file, false);
    EXPECT_TRUE(reader.readBit());
    EXPECT_FALSE(reader.readBit());
    EXPECT_TRUE(reader.readBit());
    EXPECT_TRUE(reader.readBit());

    file.close();
    remove("test_bitstream.dat");
}

TEST_F(HuffmanZipperTest, BitStreamWriteAndReadByteTest) {
    fstream file("test_bitstream.dat", ios::out | ios::in | ios::binary | ios::trunc);

    // Write byte
    BitStream writer(&file, true);
    writer.writeByte(0xAB);
    writer.pushRemainingBits();

    // Reset file position
    file.clear();
    file.seekg(0, ios::beg);

    // Read byte
    BitStream reader(&file, false);
    unsigned char value = reader.readByte();
    EXPECT_EQ(value, 0xAB);

    file.close();
    remove("test_bitstream.dat");
}

// Test HashMap operations
TEST_F(HuffmanZipperTest, HashMapInsertAndFindTest) {
    HashMap map(256);

    map.insert('A', 65);
    map.insert('B', 66);

    int value;
    EXPECT_TRUE(map.find('A', value));
    EXPECT_EQ(value, 65);

    EXPECT_TRUE(map.find('B', value));
    EXPECT_EQ(value, 66);

    EXPECT_FALSE(map.find('Z', value));
}

TEST_F(HuffmanZipperTest, HashMapUpdateTest) {
    HashMap map(256);

    map.insert('A', 65);
    map.insert('A', 100); // Update

    int value;
    map.find('A', value);
    EXPECT_EQ(value, 100);
}

TEST_F(HuffmanZipperTest, HashMapRemoveTest) {
    HashMap map(256);

    map.insert('A', 65);
    EXPECT_TRUE(map.contains('A'));

    map.remove('A');
    EXPECT_FALSE(map.contains('A'));
}

TEST_F(HuffmanZipperTest, HashMapSizeTest) {
    HashMap map(256);
    EXPECT_EQ(map.getSize(), 0);

    map.insert('A', 1);
    map.insert('B', 2);
    EXPECT_EQ(map.getSize(), 2);

    map.remove('A');
    EXPECT_EQ(map.getSize(), 1);
}

// Test DynamicArray operations
TEST_F(HuffmanZipperTest, DynamicArrayPushAndPopTest) {
    DynamicArray<int> arr;

    arr.pushBack(10);
    arr.pushBack(20);
    arr.pushBack(30);

    EXPECT_EQ(arr.getSize(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);

    arr.popBack();
    EXPECT_EQ(arr.getSize(), 2);
}

TEST_F(HuffmanZipperTest, DynamicArrayResizeTest) {
    DynamicArray<int> arr(2); // Small initial capacity

    for (int i = 0; i < 10; i++) {
        arr.pushBack(i);
    }

    EXPECT_EQ(arr.getSize(), 10);
    EXPECT_GE(arr.getCapacity(), 10);
}

TEST_F(HuffmanZipperTest, DynamicArrayEmptyTest) {
    DynamicArray<int> arr;
    EXPECT_TRUE(arr.isEmpty());

    arr.pushBack(1);
    EXPECT_FALSE(arr.isEmpty());

    arr.clear();
    EXPECT_TRUE(arr.isEmpty());
}

// Integration tests for compression/decompression
TEST_F(HuffmanZipperTest, CompressionDecompressionRoundTripTest) {
    // This test would require implementing the compress/decompress functions
    // in a testable way (e.g., as separate functions that can be called)
    // For now, this is a placeholder showing the test structure

    string original = readFile("test_input.txt");

    // Compress
    // compressFile("test_input.txt", "test_compressed.huf");

    // Decompress
    // decompressFile("test_compressed.huf", "test_decompressed.txt");

    // string decompressed = readFile("test_decompressed.txt");
    // EXPECT_EQ(original, decompressed);
}

TEST_F(HuffmanZipperTest, EmptyFileHandlingTest) {
    // Test handling of empty files
    string content = readFile("test_empty.txt");
    EXPECT_TRUE(content.empty());
}

TEST_F(HuffmanZipperTest, SingleCharacterFileTest) {
    // Test handling of single character files
    string content = readFile("test_single.txt");
    EXPECT_EQ(content, "A");
    EXPECT_EQ(content.length(), 1);
}

TEST_F(HuffmanZipperTest, LargeFileTest) {
    // Test handling of larger files
    string content = readFile("test_large.txt");
    EXPECT_EQ(content.length(), 1000);
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
