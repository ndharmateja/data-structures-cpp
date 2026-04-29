#ifndef MIN_PQ_HPP
#define MIN_PQ_HPP

#include <vector>
#include <type_traits>

template <typename T>
class MinPQ
{
    // Compile time assert to see if we can use the '<' operator on the
    // T type that is used
    static_assert(std::is_convertible<decltype(std::declval<T>() < std::declval<T>()), bool>::value,
                  "MinPQ requires type T to support operator<");

private:
    // Instance variables
    // We don't need the size variable as the vector keeps track of the number
    // of elements that is present in the array
    std::vector<T> pq;

    // Helper methods
    void sink(int i);
    void swim(int i);
    void heapify();
    static int parent(int i) { return (i - 1) / 2; }
    static int left_child(int i) { return 2 * i + 1; };
    static int right_child(int i) { return 2 * i + 2; };

public:
    MinPQ() {}
    MinPQ(int max_n) { pq.reserve(max_n); }
    MinPQ(std::vector<T> data) : pq(std::move(data)) { heapify(); }

    void insert(T t);
    T min() const;
    T delete_min();
    bool is_empty() const { return pq.empty(); }
    int size() const { return static_cast<int>(pq.size()); }
};

#endif