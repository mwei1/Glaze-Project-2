//
// Created by Zlata Kovrigina on 3/22/26.
//


#include "HashTable.h"

//found ideas for hash function from https://cp-algorithms.com/string/string-hashing.html
//hashes using rolling polynomial method to ensure good hash function
int HashTable::hash(const std::string &key) const{
    const int p = 31;
    long long hash = 0;
    long long mod = 1e9 + 9;

    for (char c : key) {
        hash = (hash * p + c) % mod;
    }
    return int(hash % cap);
}

//rehashes and doubles cap and then moves all data from one vector to the new nd bigger one
void HashTable::rehash() {
    int oldCap = cap;
    cap *= 2;
    std::vector<std::vector<Node*>> newTable(cap);

    for (int i = 0; i < oldCap; i++) {
        for (auto node : table[i]) {
            int newIndex = hash(node->key);
            newTable[newIndex].push_back(node);
        }
    }
    table = std::move(newTable);
}

//returns the load factor
double HashTable::getLoad() const {
    return static_cast<double>(size) / cap;
}

//returns the sizer
int HashTable::getSize() const {
    return size;
}

//makes a hash table with starting units
HashTable::HashTable() {
    cap= 100;
    size = 0;
    table.resize(cap);
}

//destructor
HashTable::~HashTable() {
    for (auto &bucket : table) {
        for (auto node : bucket) {
            delete node;
        }
    }
}

//isert does hash function to find bucket it should be in if previously inserted does not insert
//otherwise checks load factr to make sure its not too high then rehashes if needed
void HashTable::insert(std::string key, int index) {
    if (key.empty()) {
        return;
    }
    int i = hash(key);
    for (auto node : table[i]) {
        if (node->key == key) {
            return;
        }
    }

    Node* n = new Node{key, index};
    table[i].push_back(n);
    size++;

    if (getLoad() > .75) {
        rehash();
    }
}

//search goes through bucket that hash finds key to be in and returns vec of all found indices
std::vector<int> HashTable::search(std::string key) const {
    if (key.empty()) {
        return {};
    }
    int i = hash(key);
    std::vector<int> indices;

    for (auto bucket : table[i]) {
        if (bucket->key == key) {
            indices.push_back(bucket->index);
        }
    }
    return indices;
}
