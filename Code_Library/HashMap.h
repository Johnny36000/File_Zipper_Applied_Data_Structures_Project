#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>
using namespace std;

struct HashNode {
    char key;
    int value;
    bool occupied;
    bool deleted;

    HashNode() {
        key = '\0';
        value = 0;
        occupied = false;
        deleted = false;
    }
};

class HashMap {
private:
    HashNode* table;
    int capacity;
    int size;

    int hash(char key);
    int findIndex(char key);

public:
    HashMap(int cap = 256);
    ~HashMap();

    void insert(char key, int value);
    bool find(char key, int& value);
    bool remove(char key);
    bool contains(char key);
    void print();

    int getSize() const;
};

#endif
