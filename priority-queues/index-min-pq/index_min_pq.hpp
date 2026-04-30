#ifndef INDEX_MIN_PQ_HPP
#define INDEX_MIN_PQ_HPP

#include <vector>
#include <type_traits>
#include <algorithm>
#include <stdexcept>

template <typename T>
class IndexMinPQ
{
    // Compile time assert to see if we can use the '<' operator on the
    // T type that is used
    static_assert(std::is_convertible<decltype(std::declval<T>() < std::declval<T>()), bool>::value,
                  "IndexMinPQ requires type T to support operator<");

private:
    // Instance variables
    // of elements that is present in the array
    // pq[i]   => i is the heap index and pq[i] is the user specified index
    // keys[i] => i is the heap index and keys[i] is the actual element
    // pq and keys are parallel arrays where
    //     pq[i] holds the user specified index and
    //     keys[i] holds the corresponding item
    // qp[i] => i is the user specified index and qp[i] is the heap index
    //          qp[i] is -1 if user didn't associate i with any element
    std::vector<int> qp;
    std::vector<int> pq;
    std::vector<T> keys;
    int n;
    int max_n;

    // Helper methods
    void swap(int i, int j)
    {
        // Swap elements at heap indices i and j
        std::swap(pq[i], pq[j]);
        std::swap(keys[i], keys[j]);

        // When we swap elements at heap indices i and j
        // we also need to swap indices pq[i] and pq[j] in the qp array
        // as the heap indices corresponding to the user indices have been changed
        int user_index_i{pq[i]}, user_index_j{pq[j]};
        std::swap(qp[user_index_i], qp[user_index_j]);
    }

    void swim(int i)
    {
        while (i > 0)
        {
            // We need to keep swapping as long as the parent > child
            int p = parent(i);

            // If keys[p] <= keys[i], we can break
            // (keys[p] <= keys[i]) <=> !(keys[p] > keys[i]) <=> !(keys[i] < keys[p])
            if (!(keys[i] < keys[p]))
                break;

            // Swap i with parent and update i
            swap(i, p);
            i = p;
        }
    }

    void sink(int i)
    {
        // Run the loop as long as there is a left child
        int to_swap;
        while ((to_swap = left_child(i)) < n)
        {
            // If the right child also exists and the right child element
            // is smaller than the left child, then we need to swap ith element
            // with right child, so we increment to_swap index
            if (to_swap < n - 1 && pq[to_swap + 1] < pq[to_swap])
                to_swap++;

            // If the element at i less than or equal than the smaller child
            // then we don't need to swap
            // (keys[i] <= keys[to_swap]) <=> !(keys[i] > keys[to_swap]) <=> !(keys[to_swap] < keys[i])
            if (!(keys[to_swap] < keys[i]))
                break;

            // Swap with the smaller child and update i
            swap(i, to_swap);
            i = to_swap;
        }
    }

    static int parent(int i) { return (i - 1) / 2; }
    static int left_child(int i) { return 2 * i + 1; };
    static int right_child(int i) { return 2 * i + 2; };

public:
    // Initialize (not reserve) the vectors with the given max_n
    IndexMinPQ(int max_n) : qp(max_n, -1), pq(max_n), keys(max_n), n{0}, max_n{max_n} {}

    void insert(int k, T key)
    {
        // Error handling
        // If k is out of bounds
        if (!(0 <= k && k < max_n))
            throw std::out_of_range("Index out of bounds.");

        // If index k is already associated with an element
        if (qp[k] != -1)
            throw std::invalid_argument("Index already associated with an element.");

        // Insert the item at the end and swim
        pq[n] = k;
        keys[n] = key;
        qp[k] = n;
        n++;
        swim(n - 1);
    }

    void change(int k, T key)
    {
        // Error handling
        // If k is out of bounds
        if (!(0 <= k && k < max_n))
            throw std::out_of_range("Index out of bounds.");

        // If index k is not associated with any element
        if (qp[k] == -1)
            throw std::invalid_argument("Index not associated with any element.");

        // Change the item associated with index 'k'
        int heap_index = qp[k];
        T old_key = keys[heap_index];
        keys[heap_index] = key;

        // swim/sink appropriately based on whether the value increased or decreased
        // If value is decreased, then we only potentially need to swim as heap property
        // is potentially violated with the parent
        // Similarly we potentially need to sink otherwise
        if (key < old_key)
            swim(heap_index);
        else
            sink(heap_index);
    }

    bool contains(int k) const { return qp[k] != -1; }

    void remove(int k)
    {
        // Error handling
        // If k is out of bounds
        if (!(0 <= k && k < max_n))
            throw std::out_of_range("Index out of bounds.");

        // If index k is not associated with any element
        if (qp[k] == -1)
            throw std::invalid_argument("Index not associated with any element.");

        // Get the index of the element in the heap and set it to
        int heap_index = qp[k];

        // Swap the element with the last element, decrement the n
        // and remove the given index 'k' from qp
        swap(heap_index, n - 1);
        n--;
        qp[k] = -1;

        // Sink the index where the last element was
        sink(heap_index);
    }

    T min() const
    {
        if (!n)
            throw std::underflow_error("IndexMinPQ is empty.");
        return keys.front();
    }

    int min_index() const
    {
        if (!n)
            throw std::underflow_error("IndexMinPQ is empty.");
        return pq.front();
    }

    int remove_min()
    {
        if (!n)
            throw std::underflow_error("IndexMinPQ is empty.");

        // Get the user index of the minimal element and remove it
        int user_index = pq[0];
        remove(user_index);
        return user_index;
    }

    bool is_empty() const { return !n; }
    int size() const { return n; }
};

#endif