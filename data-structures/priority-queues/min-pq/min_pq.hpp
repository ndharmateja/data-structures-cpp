#ifndef MIN_PQ_HPP
#define MIN_PQ_HPP

#include <vector>
#include <type_traits>
#include <algorithm>
#include <stdexcept>

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
    void swim(int i)
    {
        while (i > 0)
        {
            // We need to keep swapping as long as the parent > child
            int p = parent(i);

            // If pq[p] <= pq[i], we can break
            // (pq[p] <= pq[i]) <=> !(pq[p] > pq[i]) <=> !(pq[i] < pq[p])
            if (!(pq[i] < pq[p]))
                break;

            // Swap i with parent and update i
            std::swap(pq[i], pq[p]);
            i = p;
        }
    }

    void sink(int i)
    {
        // Run the loop as long as there is a left child
        int to_swap, n{static_cast<int>(pq.size())};
        while ((to_swap = left_child(i)) < n)
        {
            // If the right child also exists and the right child element
            // is smaller than the left child, then we need to swap ith element
            // with right child, so we increment to_swap index
            if (to_swap < n - 1 && pq[to_swap + 1] < pq[to_swap])
                to_swap++;

            // If the element at i less than or equal than the smaller child
            // then we don't need to swap
            // (pq[i] <= pq[to_swap]) <=> !(pq[i] > pq[to_swap]) <=> !(pq[to_swap] < pq[i])
            if (!(pq[to_swap] < pq[i]))
                break;

            // Swap with the smaller child and update i
            std::swap(pq[i], pq[to_swap]);
            i = to_swap;
        }
    }

    void heapify()
    {
        // Start from the right most node non-leaf node in the
        // last but one level and keep sinking until we reach the root
        int n{static_cast<int>(pq.size())};
        for (int i = parent(n - 1); i >= 0; --i)
            sink(i);
    }

    static int parent(int i) { return (i - 1) / 2; }
    static int left_child(int i) { return 2 * i + 1; };
    static int right_child(int i) { return 2 * i + 2; };

public:
    MinPQ() {}
    MinPQ(int max_n) { pq.reserve(max_n); }
    MinPQ(std::vector<T> data) : pq(std::move(data)) { heapify(); }

    void insert(T t)
    {
        // Insert the item at the end and swim
        pq.push_back(t);
        swim(pq.size() - 1);
    }

    T min() const
    {
        if (pq.empty())
            throw std::underflow_error("MinPQ is empty.");
        return pq.front();
    }

    T remove_min()
    {
        if (pq.empty())
            throw std::underflow_error("MinPQ is empty.");

        // Swap the element at index 0 and size-1
        // and pop the element at index size-1 and sink the root
        T min_element = pq.front();
        std::swap(pq.front(), pq.back());
        pq.pop_back();
        sink(0);

        // Return the min element
        return min_element;
    }

    bool is_empty() const { return pq.empty(); }
    int size() const { return static_cast<int>(pq.size()); }
};

#endif