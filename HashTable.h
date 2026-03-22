//
// Created by Zlata Kovrigina on 3/22/26.
//

#ifndef GLAZEPROJECT2_HASHTABLE_H
#define GLAZEPROJECT2_HASHTABLE_H
#include <string>
#include <vector>

struct Node {
    std::string key;
    int index;
};

class HashTable {
private:
    std::vector<std::vector<Node*>> table;
    int cap;
    int size;

    int hash(const std::string& key) const;
    void rehash();

public:
    HashTable();
    ~HashTable();

    void insert(std::string key, int index);
    std::vector<int> search(std::string key) const;

    double getLoad() const;
    int getSize() const;
};


#endif //GLAZEPROJECT2_HASHTABLE_H