/** AI Generated */

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "index_min_pq.hpp"

// --- Simple Test Framework ---
int tests_passed = 0;
int tests_failed = 0;

void print_test(const std::string &name, bool passed)
{
    if (passed)
    {
        std::cout << "\033[32m[ PASS ]\033[0m " << name << std::endl;
        tests_passed++;
    }
    else
    {
        std::cout << "\033[31m[ FAIL ]\033[0m " << name << std::endl;
        tests_failed++;
    }
}

// Helper to test if a specific exception is thrown
template <typename ExceptionType, typename Function>
bool throws_exception(Function func)
{
    try
    {
        func();
        return false; // Did not throw
    }
    catch (const ExceptionType &)
    {
        return true; // Threw the correct exception
    }
    catch (...)
    {
        return false; // Threw the WRONG exception
    }
}

// --- Test Cases ---

void test_initialization()
{
    std::cout << "\n--- Test Group: Initialization & Empty State ---\n";
    IndexMinPQ<int> pq(10);

    print_test("Newly created PQ is empty", pq.is_empty() == true);
    print_test("Newly created PQ size is 0", pq.size() == 0);
    print_test("min() throws underflow", throws_exception<std::underflow_error>([&]()
                                                                                { pq.min(); }));
    print_test("remove_min() throws underflow", throws_exception<std::underflow_error>([&]()
                                                                                       { pq.remove_min(); }));
}

void test_basic_operations()
{
    std::cout << "\n--- Test Group: Basic Insertion & Retrieval ---\n";
    IndexMinPQ<std::string> pq(10);

    pq.insert(3, "Delta");
    pq.insert(1, "Bravo");
    pq.insert(0, "Alpha");
    pq.insert(2, "Charlie");

    print_test("Size is correct after 4 inserts", pq.size() == 4);
    print_test("contains(1) is true", pq.contains(1) == true);
    print_test("contains(5) is false", pq.contains(5) == false);

    print_test("min() returns smallest key ('Alpha')", pq.min() == "Alpha");
    print_test("min_index() returns index of smallest key (0)", pq.min_index() == 0);

    print_test("remove_min() returns correct index (0)", pq.remove_min() == 0);
    print_test("Size updates after remove_min", pq.size() == 3);
    print_test("min() updates to next smallest ('Bravo')", pq.min() == "Bravo");
}

void test_key_changes()
{
    std::cout << "\n--- Test Group: Key Modifications (Swim & Sink) ---\n";
    IndexMinPQ<int> pq(10);

    pq.insert(0, 50);
    pq.insert(1, 60);
    pq.insert(2, 70);

    // Test Swim (Decrease Key)
    pq.change(2, 10);
    print_test("change() triggering swim updates min correctly", pq.min() == 10 && pq.min_index() == 2);

    // Test Sink (Increase Key)
    pq.change(2, 90);
    print_test("change() triggering sink updates min correctly", pq.min() == 50 && pq.min_index() == 0);
}

void test_arbitrary_removal()
{
    std::cout << "\n--- Test Group: Arbitrary Removal ---\n";
    IndexMinPQ<int> pq(10);

    pq.insert(0, 100);
    pq.insert(1, 200);
    pq.insert(2, 300);

    pq.remove(1); // Remove middle element
    print_test("contains(1) is false after removal", pq.contains(1) == false);
    print_test("Size is correct after arbitrary removal", pq.size() == 2);
    print_test("Heap property maintained after arbitrary removal", pq.min() == 100);

    pq.remove(0); // Remove root
    print_test("min() updates correctly after root removal", pq.min() == 300 && pq.min_index() == 2);
}

void test_edge_cases_and_exceptions()
{
    std::cout << "\n--- Test Group: Edge Cases & Exceptions ---\n";
    IndexMinPQ<int> pq(5); // Max capacity 5 (indices 0-4)

    pq.insert(2, 50);

    // Out of bounds
    print_test("insert() below 0 throws out_of_range", throws_exception<std::out_of_range>([&]()
                                                                                           { pq.insert(-1, 10); }));
    print_test("insert() above max_n throws out_of_range", throws_exception<std::out_of_range>([&]()
                                                                                               { pq.insert(5, 10); }));
    print_test("change() out of bounds throws out_of_range", throws_exception<std::out_of_range>([&]()
                                                                                                 { pq.change(10, 10); }));

    // Logical errors
    print_test("insert() on existing index throws invalid_argument", throws_exception<std::invalid_argument>([&]()
                                                                                                             { pq.insert(2, 99); }));
    print_test("change() on non-existent index throws invalid_argument", throws_exception<std::invalid_argument>([&]()
                                                                                                                 { pq.change(3, 99); }));
    print_test("remove() on non-existent index throws invalid_argument", throws_exception<std::invalid_argument>([&]()
                                                                                                                 { pq.remove(3); }));
}

int main()
{
    std::cout << "====================================\n";
    std::cout << "   IndexMinPQ Comprehensive Tests   \n";
    std::cout << "====================================\n";

    test_initialization();
    test_basic_operations();
    test_key_changes();
    test_arbitrary_removal();
    test_edge_cases_and_exceptions();

    std::cout << "\n====================================\n";
    std::cout << "Test Summary: " << tests_passed << " Passed, " << tests_failed << " Failed\n";

    return (tests_failed == 0) ? 0 : 1;
}