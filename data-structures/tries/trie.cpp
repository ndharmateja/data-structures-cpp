#include "trie.hpp"

TrieNode *Trie::remove(TrieNode *node, const std::string &word, int d)
{
    if (!node)
        return nullptr;
    if (d == static_cast<int>(word.size()))
        node->is_word = false;
    else
    {
        unsigned char c = word[d];
        node->children[c - 'a'] = remove(node->children[c - 'a'], word, d + 1);
    }

    if (node->is_word)
        return node;
    for (int i = 0; i < 26; i++)
        if (node->children[i])
            return node;

    delete node;
    return nullptr;
}

void Trie::insert(const std::string &word)
{
    // If root is null we create it
    // It could be null initially in an empty trie or after deletions
    if (!root)
        root = new TrieNode();

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

bool Trie::search(const std::string &word) const
{
    // If root is null then we can return false
    if (!root)
        return false;

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
    // If root is null then we can return false
    if (!root)
        return false;

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

int Trie::len_of_longest_prefix_of(const std::string &s) const
{
    // If root is null, then there the trie is empty and we can return
    // an empty string
    int prefix_len{0};
    if (!root)
        return prefix_len;

    // Traverse the trie from the root character by character using the
    // characters of the given string and form the result
    TrieNode *curr{root};
    int child_idx, word_len{static_cast<int>(s.size())};
    for (int i = 0; i < word_len; i++)
    {
        // The invariant for this loop is that it assumes that curr is going
        // to be non null
        // Get the node corresponding to the current character
        child_idx = s[i] - 'a';
        curr = curr->children[child_idx];

        // If curr becomes nullptr then we can exit as the curr char 'c'
        // is not part of the prefix
        if (!curr)
            break;

        // At this point there exists a trie node corresponding to the curr char 'c'
        // and if it is a valid word, this would be the length of the longest prefix
        // so far
        if (curr->is_word)
            prefix_len = i + 1;
    }

    // We either broke out of the loop early reaching a null trie node
    // or we ran out of characters in the string s
    // At this point either way, the result contains the len of the
    // longest prefix of 's' that is a key of the trie
    return prefix_len;
}

std::string Trie::longest_prefix_of(const std::string &s) const
{
    // Find the length of the key that is the longest prefix of s
    // and return the actual prefix using substring
    int len{len_of_longest_prefix_of(s)};
    return s.substr(0, len);
}