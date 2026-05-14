/**
 * For the purpose of using in Leetcode
 * For fully functional tries, see trie.hpp and trie.cpp
 * See trie2.cpp for another implementation of the trie where
 * delete happens without deletion of actual nodes.
 *
 * 1. This trie has a very efficient implementation (iterative) of
 *    search and starts_with functions
 * 2. Nodes are not deleted upon delete, only the sizes are updated.
 *    This is for the word search 2 problem where we need to prune the paths
 *    but not actually lose access to the nodes.
 * 3. The search function takes into account that just being a non-null node
 *    doesn't mean that it is a valid trie node in the tree. Its count has to be > 0.
 * 4. The root is never null as the nodes once created are never deleted.
 * 5. Works only for lowercase english alphabet.
 */

#include <string>
#include <array>

struct TrieNode
{
    bool is_word;
    int n;
    std::array<TrieNode *, 26> children;

    TrieNode() : is_word{false}, n{0}
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

    /**
     * Recursively helper method to insert
     * Invariant is that node (if exists) would correspond to the word[:d] prefix
     * and if the node doesn't exist, it would be created.
     *
     * ! The given node need not be null unlike the invariants of the remove
     * ! collect_keys_that_match methods
     */
    TrieNode *insert(TrieNode *node, const std::string &word, int d)
    {
        // The invariant is that node (if exists) corresponds to the word[:d] prefix
        // of the word
        // If the node doesn't exist then we create it (with a count 0)
        if (!node)
            node = new TrieNode();

        // If d == word's length, it means that this is the node that corresponds to
        // word[:d] = word[:word_len] which is the full word
        // which means if this wasn't already a word, then we make it a word
        // and exit (after incrementing the count)
        if (d == static_cast<int>(word.size()))
        {
            // If it is already a word, then this word already exists
            // and we don't need to do anything at all
            if (node->is_word)
                return node;

            // At this point the node is not a valid word, so we make it a valid word
            // and increment its count and exit the function as there is nothing else to do
            node->is_word = true;
            node->n++;
            return node;
        }

        // If d < word's length, we need to continue the insertion
        int child_idx = word[d] - 'a';

        // ! Note that child_node is a reference to the element at the
        // ! child_idx in the children vector
        TrieNode *&child_node = node->children[child_idx];

        // We are storing the old count where we need to insert to figure out if we need to
        // increment the count of the current node
        int old_count = child_node ? child_node->n : 0;
        child_node = insert(child_node, word, d + 1);

        // If the child's count changed after the insertion, it means that a new key was
        // successfully added to the trie (as a descendant of the current node)
        // so we increment the count of the current node and return it
        if (child_node->n != old_count)
            node->n++;
        return node;
    }

    /**
     * Recursive helper method to remove
     * Invariant is that we assume that node is going to be not null
     * (to prevent an extra function call for the null check)
     * and word[:d] is the string corresponding to the given node
     *
     * ! We don't need the remove function to return the pointer of the
     * ! new root of the subtrie as we are not deleting nodes. We are only
     * ! decrementing the counts
     */
    void remove(TrieNode *node, const std::string &word, int d)
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
            return;
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
                remove(child_node, word, d + 1);

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
                if (child_node->n != old_child_n)
                    node->n--;
            }
        }
    }

public:
    Trie() : root{new TrieNode()} {}
    ~Trie() { delete root; }

    TrieNode *get_root() { return root; }

    /**
     * ! Root is never null. So we can directly return root's size.
     */
    int size() { return root->n; }
    bool empty() { return root->n == 0; }

    /**
     * Inserts the given word into the trie as a key
     */
    void insert(const std::string &word) { root = insert(root, word, 0); }

    /**
     * Removes the given word from the trie if it exists
     * and does nothing otherwise
     *
     * ! Root is never going to be null, so we wouldn't need to update it
     */
    void remove(const std::string &word) { remove(root, word, 0); }

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

            // ! We only search subtries whose root has atleast a count of 1
            if (!curr || !curr->n)
                return false;
        }

        // Say the word is EAT and all intermediate trienodes exist along the path
        // At this point curr will point to the node forming EAT
        // so if this node represents a full word, we can return true
        // false otherwise
        return curr->is_word;
    }

    bool starts_with(const std::string &prefix) const
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

            // ! We only search subtries whose root has atleast a count of 1
            if (!curr || !curr->n)
                return false;
        }

        // At this point we know the trie node corresponding to the prefix exists
        // as we'd have returned false otherwise
        // We can directly return true
        return true;
    }
};
