#ifndef FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_HASHMAP_H
#define FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_HASHMAP_H
#include "DynamicArray.h"

/**
 * HashNode - Node structure for hash map entries
 */
template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    bool occupied;
    bool deleted;

    HashNode() : occupied(false), deleted(false) {}
};

/**
 * HashMap class - Hash table implementation for storing key-value pairs
 * Uses open addressing with linear probing for collision resolution
 */
template <typename K, typename V>
class HashMap {
private:
    HashNode<K, V>* table;
    int capacity;
    int size;
    int deletedCount;

    // Hash function
    int hashFunction(K key);

    // Helper functions
    void resize();
    int findSlot(K key, bool forInsertion);

public:
    // Constructor and Destructor
    HashMap(int cap = 256);
    ~HashMap();

    // Core operations
    void insert(K key, V value);
    bool find(K key, V& value);
    bool remove(K key);

    // Utility functions
    int getSize();
    bool isEmpty();
    void clear();

    // Iterator support
    class Iterator {
    private:
        HashNode<K, V>* tablePtr;
        int currentIndex;
        int tableCapacity;

        void moveToNext();

    public:
        Iterator(HashNode<K, V>* table, int index, int cap);
        bool hasNext();
        void next();
        K getKey();
        V getValue();
    };

    Iterator getIterator();
};

// Template implementation must be in header file

/**
 * Constructor
 */
template <typename K, typename V>
HashMap<K, V>::HashMap(int cap) {
    capacity = cap;
    size = 0;
    deletedCount = 0;
    table = new HashNode<K, V>[capacity];
}

/**
 * Destructor
 */
template <typename K, typename V>
HashMap<K, V>::~HashMap() {
    delete[] table;
}

/**
 * Hash function for various key types
 */
template <typename K, typename V>
int HashMap<K, V>::hashFunction(K key) {
    // For unsigned char (byte)
    unsigned long hash = 5381;
    unsigned char* ptr = (unsigned char*)&key;
    for (int i = 0; i < sizeof(K); i++) {
        hash = ((hash << 5) + hash) + ptr[i];
    }
    return hash % capacity;
}

/**
 * Find slot for key (for insertion or lookup)
 */
template <typename K, typename V>
int HashMap<K, V>::findSlot(K key, bool forInsertion) {
    int index = hashFunction(key);
    int firstDeleted = -1;
    int probeCount = 0;

    while (probeCount < capacity) {
        if (!table[index].occupied) {
            // Empty slot found
            if (forInsertion && firstDeleted != -1) {
                return firstDeleted;
            }
            return forInsertion ? index : -1;
        }

        if (table[index].deleted) {
            // Mark first deleted slot for insertion
            if (forInsertion && firstDeleted == -1) {
                firstDeleted = index;
            }
        } else if (table[index].key == key) {
            // Key found
            return index;
        }

        // Linear probing
        index = (index + 1) % capacity;
        probeCount++;
    }

    return firstDeleted;
}

/**
 * Resize hash table when load factor is high
 */
template <typename K, typename V>
void HashMap<K, V>::resize() {
    int oldCapacity = capacity;
    HashNode<K, V>* oldTable = table;

    capacity *= 2;
    table = new HashNode<K, V>[capacity];
    size = 0;
    deletedCount = 0;

    // Rehash all elements
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].occupied && !oldTable[i].deleted) {
            insert(oldTable[i].key, oldTable[i].value);
        }
    }

    delete[] oldTable;
}

/**
 * Insert key-value pair
 */
template <typename K, typename V>
void HashMap<K, V>::insert(K key, V value) {
    // Resize if load factor > 0.7
    if ((size + deletedCount) * 10 > capacity * 7) {
        resize();
    }

    int index = findSlot(key, true);

    if (index != -1) {
        if (table[index].occupied && !table[index].deleted && table[index].key == key) {
            // Update existing key
            table[index].value = value;
        } else {
            // Insert new key
            if (table[index].deleted) {
                deletedCount--;
            }
            table[index].key = key;
            table[index].value = value;
            table[index].occupied = true;
            table[index].deleted = false;
            size++;
        }
    }
}

/**
 * Find value by key
 */
template <typename K, typename V>
bool HashMap<K, V>::find(K key, V& value) {
    int index = findSlot(key, false);

    if (index != -1 && !table[index].deleted) {
        value = table[index].value;
        return true;
    }

    return false;
}

/**
 * Remove key-value pair
 */
template <typename K, typename V>
bool HashMap<K, V>::remove(K key) {
    int index = findSlot(key, false);

    if (index != -1 && !table[index].deleted) {
        table[index].deleted = true;
        size--;
        deletedCount++;
        return true;
    }

    return false;
}

/**
 * Get current size
 */
template <typename K, typename V>
int HashMap<K, V>::getSize() {
    return size;
}

/**
 * Check if empty
 */
template <typename K, typename V>
bool HashMap<K, V>::isEmpty() {
    return size == 0;
}

/**
 * Clear all elements
 */
template <typename K, typename V>
void HashMap<K, V>::clear() {
    for (int i = 0; i < capacity; i++) {
        table[i].occupied = false;
        table[i].deleted = false;
    }
    size = 0;
    deletedCount = 0;
}

// Iterator Implementation

template <typename K, typename V>
HashMap<K, V>::Iterator::Iterator(HashNode<K, V>* table, int index, int cap) {
    tablePtr = table;
    currentIndex = index;
    tableCapacity = cap;
    moveToNext();
}

template <typename K, typename V>
void HashMap<K, V>::Iterator::moveToNext() {
    while (currentIndex < tableCapacity &&
           (!tablePtr[currentIndex].occupied || tablePtr[currentIndex].deleted)) {
        currentIndex++;
    }
}

template <typename K, typename V>
bool HashMap<K, V>::Iterator::hasNext() {
    return currentIndex < tableCapacity;
}

template <typename K, typename V>
void HashMap<K, V>::Iterator::next() {
    currentIndex++;
    moveToNext();
}

template <typename K, typename V>
K HashMap<K, V>::Iterator::getKey() {
    return tablePtr[currentIndex].key;
}

template <typename K, typename V>
V HashMap<K, V>::Iterator::getValue() {
    return tablePtr[currentIndex].value;
}

template <typename K, typename V>
typename HashMap<K, V>::Iterator HashMap<K, V>::getIterator() {
    return Iterator(table, 0, capacity);
}

#endif //FILE_ZIPPER_APPLIED_DATA_STRUCTURES_PROJECT_HASHMAP_H