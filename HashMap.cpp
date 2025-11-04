#include "HashMap.h"

HashMap::HashMap(int cap) {
    capacity = cap;
    size = 0;
    table = new HashNode[capacity];
}

HashMap::~HashMap() {
    delete[] table;
}

int HashMap::hash(char key) {

    return (int)key % capacity;
}

int HashMap::findIndex(char key) {
    int index = hash(key);
    int start = index;

    while (table[index].occupied) {
        if (!table[index].deleted && table[index].key == key)
            return index;
        index = (index + 1) % capacity;
        if (index == start)
            break; // table full
    }
    return -1;
}

void HashMap::insert(char key, int value) {
    int index = hash(key);

    while (table[index].occupied && !table[index].deleted && table[index].key != key) {
        index = (index + 1) % capacity;
    }

    if (!table[index].occupied || table[index].deleted) {
        table[index].key = key;
        table[index].value = value;
        table[index].occupied = true;
        table[index].deleted = false;
        size++;
    } else {
        table[index].value = value; // update
    }
}

bool HashMap::find(char key, int& value) {
    int index = findIndex(key);
    if (index != -1) {
        value = table[index].value;
        return true;
    }
    return false;
}

bool HashMap::remove(char key) {
    int index = findIndex(key);
    if (index != -1) {
        table[index].deleted = true;
        size--;
        return true;
    }
    return false;
}

bool HashMap::contains(char key) {
    int dummy;
    return find(key, dummy);
}

int HashMap::getSize() const {
    return size;
}

void HashMap::print() {
    cout << "HashMap contents:\n";
    for (int i = 0; i < capacity; i++) {
        if (table[i].occupied && !table[i].deleted) {
            cout << table[i].key << " : " << table[i].value << endl;
        }
    }
}
