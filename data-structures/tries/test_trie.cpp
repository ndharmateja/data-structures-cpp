#include <iostream>
#include <string>
#include <cassert>
#include "trie.hpp"

// Assuming your Trie class is defined here or included via a header
// #include "Trie.h"

void testEmptyTrie()
{
    Trie trie;
    assert(trie.search("apple") == false);
    assert(trie.startsWith("a") == false);
    assert(trie.search("") == false);
    assert(trie.startsWith("") == false);

    // Removing from an empty trie shouldn't crash
    trie.remove("apple");
    std::cout << "testEmptyTrie passed.\n";
}

void testBasicInsertAndSearch()
{
    Trie trie;
    trie.insert("hello");

    assert(trie.search("hello") == true);
    assert(trie.search("hell") == false);   // Prefix exists, but not as a full word
    assert(trie.search("helloo") == false); // Word is a prefix of the search term
    assert(trie.search("world") == false);  // Completely unrelated word
    std::cout << "testBasicInsertAndSearch passed.\n";
}

void testStartsWith()
{
    Trie trie;
    trie.insert("programming");

    assert(trie.startsWith("p") == true);
    assert(trie.startsWith("pro") == true);
    assert(trie.startsWith("programming") == true); // A word is a prefix of itself
    assert(trie.startsWith("gram") == false);       // Substring, but not a prefix
    assert(trie.startsWith("programmingly") == false);
    std::cout << "testStartsWith passed.\n";
}

void testOverlappingWords()
{
    Trie trie;
    trie.insert("app");
    trie.insert("apple");

    assert(trie.search("app") == true);
    assert(trie.search("apple") == true);
    assert(trie.startsWith("appl") == true);
    std::cout << "testOverlappingWords passed.\n";
}

void testRemovalBasic()
{
    Trie trie;
    trie.insert("testing");
    trie.insert("tester");

    trie.remove("testing");
    assert(trie.search("testing") == false);
    assert(trie.search("tester") == true);   // Sibling word should still exist
    assert(trie.startsWith("test") == true); // Prefix should still exist because of "tester"

    trie.remove("tester");
    assert(trie.search("tester") == false);
    assert(trie.startsWith("test") == false); // No words left with this prefix
    std::cout << "testRemovalBasic passed.\n";
}

void testRemovalOverlappingPrefixFirst()
{
    Trie trie;
    // Removing a short word that is a prefix of a longer word
    trie.insert("app");
    trie.insert("apple");

    trie.remove("app");
    assert(trie.search("app") == false);    // "app" is no longer a word
    assert(trie.search("apple") == true);   // "apple" should still exist
    assert(trie.startsWith("app") == true); // "app" is still a prefix for "apple"
    std::cout << "testRemovalOverlappingPrefixFirst passed.\n";
}

void testRemovalOverlappingLongerFirst()
{
    Trie trie;
    // Removing a long word that has a shorter word as a prefix
    trie.insert("app");
    trie.insert("apple");

    trie.remove("apple");
    assert(trie.search("apple") == false);
    assert(trie.search("app") == true);       // "app" should still be a word
    assert(trie.startsWith("appl") == false); // The path to 'e' should theoretically be cleaned up
    std::cout << "testRemovalOverlappingLongerFirst passed.\n";
}

void testRemoveNonExistentWord()
{
    Trie trie;
    trie.insert("cat");

    // Removing a word that shares a prefix but isn't inserted
    trie.remove("car");
    assert(trie.search("cat") == true);

    // Removing a prefix of an existing word
    trie.remove("ca");
    assert(trie.search("cat") == true);
    std::cout << "testRemoveNonExistentWord passed.\n";
}

void testEmptyStringEdgeCase()
{
    Trie trie;
    // Depending on implementation, "" might be valid or ignored.
    // Usually, "" is handled cleanly at the root node.
    trie.insert("");
    assert(trie.search("") == true);
    assert(trie.startsWith("") == true);

    trie.remove("");
    assert(trie.search("") == false);
    std::cout << "testEmptyStringEdgeCase passed.\n";
}

int main()
{
    std::cout << "Running Trie tests...\n";
    std::cout << "-----------------------\n";

    testEmptyTrie();
    testBasicInsertAndSearch();
    testStartsWith();
    testOverlappingWords();
    testRemovalBasic();
    testRemovalOverlappingPrefixFirst();
    testRemovalOverlappingLongerFirst();
    testRemoveNonExistentWord();
    testEmptyStringEdgeCase();

    std::cout << "-----------------------\n";
    std::cout << "All tests passed successfully!\n";

    return 0;
}