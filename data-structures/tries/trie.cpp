#include "trie.hpp"

TrieNode *Trie::remove(TrieNode *node, const std::string &word, int d)
{
    // If d equals to the word's length
    // then it means that word[:d] = word[:word_len] (according to the invariant)
    // is the word corresponding to the given node which is the full word
    // So if this node corresponds to a full word, we can mark it not a valid word
    // and decrement the count of this node
    if (d == static_cast<int>(word.size()) && node->is_word)
    {
        node->is_word = false;
        node->n--;
    }

    // If d < word's length, it means that we still haven't reached the end of the word
    // So we check if the appropriate child link is present and call delete on that
    // recursively
    else
    {
        // ! Note that child_node is a reference to the child_idx index in the
        // ! children vector so that we can avoid further lookups while accessing/updating
        int child_idx = word[d] - 'a';
        TrieNode *&child_node = node->children[child_idx];
        if (child_node)
        {
            // Store the old count to check if the current node's count
            // should be updated
            int old_child_n = child_node->n;
            child_node = remove(child_node, word, d + 1);

            // Get the new child's count and update curr node's count accordingly
            // There is a possibility that the child's count won't change
            // if nothing was deleted, in which case we don't have
            // There is also the possibility that the remove method returns nullptr
            // if the child node was removed from the trie
            // We update the count of the curr node
            // The only possibility of change is that the child's size decreases by 1
            // or if the child gets deleted
            // Either way we need to just decrement the curr node's size by 1

            // ! int new_child_n = child_node ? child_node->n : 0
            // ! if (new_child_n < old_child_n)
            // !    node->n--;
            // ! This should be the actual code but what it essentially boils down to
            // ! is that if the child node becomes null (it wasn't null earlier as we only
            // ! try to delete the child node if it is not null) or if the size of the child
            // ! node after delete is not the same as the old count, we can decrement
            // ! the curr node's count.
            if (!child_node || child_node->n != old_child_n)
                node->n--;

            // ! Optimization: This isn't strictly necessary as this logic would be
            // ! handled at the end of the loop, but if the child node turns out to be
            // ! not null, then we can safely know that this node isn't going to be deleted
            // ! so we can return the node itself
            if (child_node)
                return node;
        }
    }

    // This logic is to check if we even need to delete this particular node
    // The invariant of the trie is that every leaf should be a valid word
    // so if a leaf gets deleted, all the nodes along the path to root that don't have
    // and other children and aren't valid words themselves should also be deleted

    // If this node is a word, then we shouldn't delete it
    if (node->is_word)
        return node;

    // If any of the children exist, then we shouldn't delete this node
    for (int i = 0; i < 26; i++)
        if (node->children[i])
            return node;

    // If we reach here it means that the current node is neither a valid word
    // nor have any children after the deletion happens
    // So we can safely remove this node from our trie and since we are deleting
    // this node we return nullptr so that its parent could accordingly
    // set its child to nullptr
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

void Trie::collect_keys_that_match(TrieNode *node,
                                   std::string &buffer,
                                   const int i, const int pattern_len,
                                   const std::string &pattern,
                                   std::vector<std::string> &result) const
{
    // If the value of i is same as the pattern_len
    // then we had reached the end of the pattern and we can add the
    // word corresponding to the current trie node if it is a valid word
    // Even if it is not a valid, we need to exit the function call
    // as there is no more recursion to do
    if (i == pattern_len)
    {
        if (!node->is_word)
            return;
        result.push_back(buffer);
        return;
    }

    // If i is smaller than the pattern's length, we still have to
    // traverse deeper in to the tree
    unsigned char c = pattern[i];

    // If the character is a wild card "."
    // then we have to traverse through all the valid children of the curr node
    if (c == '.')
    {
        // We will traverse through every non-null child of the current node
        for (int child_idx = 0; child_idx < 26; child_idx++)
        {
            // Get the child node and if it is null, we can skip it
            // We need to maintain the invariant, so we don't call the
            // recursive call on an empty node
            TrieNode *child = node->children[child_idx];
            if (!child)
                continue;

            // Add the char corresponding to the child node to the buffer
            // to keep the invariant
            // buffer has the i chars corresponding to the current node
            // and in the next recursive call we will add the next char
            // so that the buffer will have the first i + 1 chars corresponding
            // to the child node
            unsigned char child_char = child_idx + 'a';
            buffer.push_back(child_char);
            collect_keys_that_match(child, buffer, i + 1, pattern_len, pattern, result);

            // Remove the last char added to the buffer
            // so that we can traverse to the next child whilst maintaining the invariant
            buffer.pop_back();
        }

        // Once we have traversed through all the children of the current node
        // we can return as there is no more recursion to do
        return;
    }

    // If the character is not a wild card "."
    // then we only traverse the child corresponding to the character
    int child_idx = c - 'a';
    TrieNode *child = node->children[child_idx];

    // If the child is null, then it means we don't have it in our trie
    if (!child)
        return;

    // Add the char corresponding to the child node to the buffer
    // and traverse the child node recursively and pop the char off the buffer
    unsigned char child_char = child_idx + 'a';
    buffer.push_back(child_char);
    collect_keys_that_match(child, buffer, i + 1, pattern_len, pattern, result);
    buffer.pop_back();
}

std::vector<std::string> Trie::keys_that_match(const std::string &pattern) const
{
    // Create the result vector to collect the matching keys
    // If the root itself is null, then there are no keys
    // so we can exit
    std::vector<std::string> result;
    if (!root)
        return result;

    // Create the buffer required to collect the keys
    // The max length of the buffer is the length of the pattern
    // so we reserve it
    int pattern_len{static_cast<int>(pattern.size())};
    std::string buffer;
    buffer.reserve(pattern_len);

    // Collect the keys that match the pattern by traversing the trie
    collect_keys_that_match(root, buffer, 0, pattern_len, pattern, result);
    return result;
}