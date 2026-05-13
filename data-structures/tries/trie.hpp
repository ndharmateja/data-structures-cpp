#ifndef TRIE_HPP
#define TRIE_HPP

#include <string>
#include <array>

struct TrieNode
{
    bool is_word;
    std::array<TrieNode *, 26> children;
    TrieNode() : is_word{false}
    {
        for (int i = 0; i < 26; i++)
            children[i] = nullptr;
    }

    /**
     * TODO: Memory pool (a single vector<TrieNode>) to manage lifetime
     */
    ~TrieNode()
    {
        for (auto ptr : children)
            delete ptr;
    }
};

class Trie
{
private:
    TrieNode *root;
    TrieNode *remove(TrieNode *node, const std::string &word, int d);

public:
    Trie() : root{nullptr} {}
    ~Trie() { delete root; }

    void insert(const std::string &word);
    bool search(const std::string &word) const;
    bool starts_with(const std::string &prefix) const;
    void remove(const std::string &word) { root = remove(root, word, 0); }
};

#endif