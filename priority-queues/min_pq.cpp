#include "min_pq.hpp"
#include <stdexcept>

template <typename T>
void MinPQ<T>::heapify()
{
    // Start from the right most node non-leaf node in the
    // last but one level and keep sinking until we reach the root
    int n{static_cast<int>(pq.size())};
    for (int i = parent(n - 1); i >= 0; --i)
        sink(i);
}

template <typename T>
void MinPQ<T>::insert(T t)
{
    // Insert the item at the end and swim
    pq.push_back(t);
    swim(pq.size() - 1);
}

template <typename T>
T MinPQ<T>::min() const
{
    if (pq.empty())
        throw std::underflow_error("MinPQ is empty.");
    return pq.front();
}

template <typename T>
T MinPQ<T>::delete_min()
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

template <typename T>
void MinPQ<T>::swim(int i)
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

template <typename T>
void MinPQ<T>::sink(int i)
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
