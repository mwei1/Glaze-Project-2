//
// Created by lawre on 3/2/2026.
//

#ifndef GLAZE_PROJECT_2_TRIE_H
#define GLAZE_PROJECT_2_TRIE_H
#include <string>
#include <unordered_map>
#include <vector>

class TrieNode {
public:
    // not using TrieNode* children[26] because each dessert name can contain spaces, and upper and lower case
    //to account for all an array of size 70 would be needed, which is a waste of memory
    //for the map, memory is only used when a character appears
    std::unordered_map<char, TrieNode*> children;
    bool endOfWord = false;
    std::vector<int> recipeIndices; //a vector to handle duplicates, but there should be none
};


class Trie {
private:
    TrieNode* root;
    //this function starts at a node and explores everything below it. It is recursive, so it first checks if the node
    //is the end of a word. If it is then the recipes indices are added.We do continue to visit every child, recursively, to the end of the subtree
    void prefix_helper(TrieNode* node, std::vector<int>& result) {
        if (node->endOfWord == true) {
            for (int index : node->recipeIndices) {
                result.push_back(index);
            }
        }
        for (auto& child : node->children) {
            prefix_helper(child.second, result);
        }
    }
public:
    Trie() { root = new TrieNode(); }
    void insert(const std::string& word, int index) {
        TrieNode* curr = root;
        for (char c: word) {
            if (curr->children.find(c) == curr->children.end()) { //find(key) function returns an iterator to the element with the specified key, or the end() iterator if the key is not found
                curr->children[c] = new TrieNode();
            }
            curr = curr->children[c];
        }
        curr->endOfWord = true;
        curr->recipeIndices.push_back(index);
    }
    std::vector<int> search(const std::string& word) {
        TrieNode* curr = root;
        for (char c: word) {
            if (curr->children.find(c) == curr->children.end()) {
                return {};
            }
            curr = curr->children[c];
        }
        if (curr->endOfWord == true) {
            return curr->recipeIndices;
        }
        return {};
    }

    std::vector<int> searchByPrefix(const std::string& prefix) {
        TrieNode* curr = root;
        for (char c : prefix) {
            if (curr->children.find(c) == curr->children.end()) {
                return {};
            }
            curr = curr->children[c];
        }
        std::vector<int> result;
        prefix_helper(curr, result);
        return result;
    }
};


#endif //GLAZE_PROJECT_2_TRIE_H