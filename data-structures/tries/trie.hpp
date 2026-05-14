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
    TrieNode() : is_word{false}, n{0}
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

// TODO: 26 is now hardcoded. Custom alphabet.
// TODO: is there a word matching the given pattern? say "c.t.."

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

    /**
     * Recursively helper method to insert
     * Invariant is that node (if exists) would correspond to the word[:d] prefix
     * and if the node doesn't exist, it would be created.
     *
     * ! The given node need not be null unlike the invariants of the remove
     * ! collect_keys_that_match methods
     */
    TrieNode *insert(TrieNode *node, const std::string &word, int d);

    /**
     * Recursive helper method to remove
     * Invariant is that we assume that node is going to be not null
     * (to prevent an extra function call for the null check)
     * and word[:d] is the string corresponding to the given node
     */
    TrieNode *remove(TrieNode *node, const std::string &word, int d);

    /**
     * Returns the length of the longest prefix of the given string
     * that is a key in the trie
     */
    int len_of_longest_prefix_of(const std::string &s) const;

    /**
     * Recursive helper method to collect the keys that match the pattern.
     * Returns all the keys in the trie that match the pattern exactly
     * starting from the given node.
     * Invariant is that we assume that s[:i] is the string corresponding
     * to the node and is the first 'i' chars of the pattern
     * and that the buffer's length is also going to be 'i'
     * Also assumes that node is not null to prevent an extra funcion call.
     */
    void collect_keys_that_match(TrieNode *node,
                                 std::string &buffer,
                                 const int i, const int pattern_len,
                                 const std::string &pattern,
                                 std::vector<std::string> &result) const;

    /**
     * Recursive helper method to collect all the keys that are contained
     * in the subtrie rooted at the given node
     * Invariant is that node is assumed to be non-null and the string
     * in the buffer is the string corresponding to the given node
     */
    void collect_all_keys(TrieNode *node, std::string &buffer, std::vector<std::string> &result) const;

public:
    Trie() : root{nullptr} {}
    ~Trie() { delete root; }

    /**
     * Inserts the given word into the trie as a key
     */
    void insert(const std::string &word) { root = insert(root, word, 0); }

    /**
     * Returns true if the given word exists in the trie as a key
     * and false otherwise
     */
    bool search(const std::string &word) const;

    /**
     * Returns true if the given prefix exists in the trie (not necessarily as a key)
     * and false otherwise
     */
    bool starts_with(const std::string &prefix) const;

    /**
     * Removes the given word from the trie if it exists
     * and does nothing otherwise
     */
    void remove(const std::string &word);

    /**
     * Returns true if the number of keys in the trie is 0
     * and false otherwise
     */
    bool empty() const { return !root; }

    /**
     * Returns the number of keys (valid words) in the trie
     */
    int size() const { return root ? root->n : 0; }

    // character based operations
    /**
     * Returns the longest prefix of the given string that is a valid word
     * in the trie
     */
    std::string longest_prefix_of(const std::string &s) const;

    /**
     * Return all the keys in the trie that have the given string
     * as a prefix
     */
    std::vector<std::string> keys_with_prefix(const std::string &s) const;

    /**
     * Returns all the keys in the trie
     */
    std::vector<std::string> keys() const;

    /**
     * Returns all the keys in the trie that match the pattern exactly.
     * "." is used as a wild card.
     * Eg: "c.t" could return {"cat", "cot", "cut"}
     */
    std::vector<std::string> keys_that_match(const std::string &pattern) const;
};

#endif