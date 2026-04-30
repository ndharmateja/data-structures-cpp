#include <iostream>
#include <vector>
#include <string>
#include "min_pq.hpp"

// Simple helper to print test results
void print_test(const std::string &name, bool passed)
{
    std::cout << "[ " << (passed ? "PASS" : "FAIL") << " ] " << name << std::endl;
}

int main()
{
    std::cout << "--- Starting MinPQ Tests ---\n"
              << std::endl;

    // Test 1: Basic Insertion and Min retrieval
    try
    {
        MinPQ<int> pq;
        pq.insert(50);
        pq.insert(10);
        pq.insert(30);
        pq.insert(20);

        print_test("Size check after 4 inserts", pq.size() == 4);
        print_test("Min retrieval (expected 10)", pq.min() == 10);

        // Test 2: Extraction Order (Should be 10, 20, 30, 50)
        bool order_correct = true;
        if (pq.remove_min() != 10)
            order_correct = false;
        if (pq.remove_min() != 20)
            order_correct = false;
        if (pq.remove_min() != 30)
            order_correct = false;
        if (pq.remove_min() != 50)
            order_correct = false;

        print_test("Extraction order 10 -> 20 -> 30 -> 50", order_correct);
        print_test("Empty check after all deletions", pq.is_empty());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in Basic Test: " << e.what() << std::endl;
    }

    // Test 3: O(n) Heapify Constructor
    try
    {
        std::vector<int> data = {100, 19, 36, 17, 3, 25, 1, 2, 7};
        MinPQ<int> pq_heavy(data);

        print_test("Heapify constructor size", pq_heavy.size() == 9);
        print_test("Heapify root is smallest (expected 1)", pq_heavy.min() == 1);

        // Ensure it drains in sorted order
        int last = pq_heavy.remove_min();
        bool sorted = true;
        while (!pq_heavy.is_empty())
        {
            int current = pq_heavy.remove_min();
            if (current < last)
                sorted = false;
            last = current;
        }
        print_test("Heapify preserves heap property through drain", sorted);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in Heapify Test: " << e.what() << std::endl;
    }

    // Test 4: Custom Object Testing (Strings)
    try
    {
        MinPQ<std::string> pq_str;
        pq_str.insert("zebra");
        pq_str.insert("apple");
        pq_str.insert("mango");

        print_test("String PQ min (expected 'apple')", pq_str.min() == "apple");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in String Test: " << e.what() << std::endl;
    }

    std::cout << "\n--- Tests Complete ---" << std::endl;
    return 0;
}