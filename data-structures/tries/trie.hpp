#ifndef TRIE_HPP
#define TRIE_HPP

#include <string>
#include <array>
#include <vector>

struct TrieNode
{
    bool is_word;
    std::array<TrieNode *, 26> children;
    int n;
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

/**
 * The invariant we are going to maintain is that every leaf is a valid word.
 * So every time we delete a leaf we are going to make sure that we keep
 * deleting the trie nodes along the path to the root until we reach a non-leaf
 * or a trie node with a valid word.
 * So when there are no elements in the trie, the root will be nullptr.
 */
class Trie
{
private:
    TrieNode *root;
    TrieNode *remove(TrieNode *node, const std::string &word, int d);
    int len_of_longest_prefix_of(const std::string &s) const;

public:
    Trie() : root{nullptr} {}
    ~Trie() { delete root; }

    void insert(const std::string &word);
    bool search(const std::string &word) const;
    bool starts_with(const std::string &prefix) const;
    void remove(const std::string &word) { root = remove(root, word, 0); }

    bool empty() const { return !root; }
    int size() const { return root ? root->n : 0; }

    // character based operations
    /**
     * Returns the longest prefix of the given string that is a valid word
     * in the trie
     */
    std::string longest_prefix_of(const std::string &s) const;
    std::vector<std::string> keys_with_prefix(const std::string &s) const;
    std::vector<std::string> keys_that_match(const std::string &s) const;
};

#endif