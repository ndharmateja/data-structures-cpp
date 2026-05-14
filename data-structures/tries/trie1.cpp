/**
 * For the purpose of using in Leetcode
 * For fully functional tries, see trie.hpp and trie.cpp
 * See trie2.cpp for another implementation of the trie where
 * delete happens without deletion of actual nodes.
 *
 * 1. This trie has a very efficient implementation (iterative) of
 *    insert, search and starts_with unctions
 * 2. Doesn't maintain any size in the trie nodes.
 * 3. Doesn't have a delete functionality.
 * 4. The root is never null.
 * 5. Works only for lowercase english alphabet.
 */

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
    Trie() : root{new TrieNode()} {}
    ~Trie() { delete root; }

public:
    void insert(const std::string &word)
    {
        // Traverse along the trie following each character
        // and insert new nodes along the path if there aren't any
        int idx;
        TrieNode *curr{root};
        for (unsigned char c : word)
        {
            // We use the reference to the child as we can get away only with one array access
            // instead of potentially three array accesses
            idx = c - 'a';
            TrieNode *&child = curr->children[idx];
            if (!child)
                child = new TrieNode();

            // Update the curr pointer
            curr = child;
        }

        // At this point the trie node corresponds the given word
        // Whether or not it was already there, at this point this node
        // represents a full word, so we mark the is_word to be true
        curr->is_word = true;
    }

    bool search(const std::string &word) const
    {
        // Traverse along the trie following the characters
        int idx;
        TrieNode *curr{root};
        for (unsigned char c : word)
        {
            // Check if the ith char in the string exists as a key in the
            // children of the curr node
            // If it doesn't exist, we can immediately return false
            // We keep continuing until we reach the last char
            idx = c - 'a';
            curr = curr->children[idx];
            if (!curr)
                return false;
        }

        // Say the word is EAT and all intermediate trienodes exist along the path
        // At this point curr will point to the node forming EAT
        // so if this node represents a full word, we can return true
        // false otherwise
        return curr->is_word;
    }

    bool Trie::starts_with(const std::string &prefix) const
    {
        // Traverse along the trie following the characters
        int idx;
        TrieNode *curr{root};
        for (unsigned char c : prefix)
        {
            // Check if the ith char in the string exists as a key in the
            // children of the curr node
            // If it doesn't exist, we can immediately return false
            // We keep continuing until we reach the last char
            idx = c - 'a';
            curr = curr->children[idx];
            if (!curr)
                return false;
        }

        // At this point we know the trie node corresponding to the prefix exists
        // as we'd have returned false otherwise
        // We can directly return true
        return true;
    }
};
