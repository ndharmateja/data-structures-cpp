## First approach:
### Implementation
- `pq[i]`   => `i` is the heap index and `pq[i]` is the user specified index
- `keys[i]` => `i` is the heap index and `keys[i]` is the actual element
- `pq` and `keys` are parallel arrays where
  - `pq[i]` holds the user specified index and
  - `keys[i]` holds the corresponding item
- `qp[i]` => `i` is the user specified index and `qp[i]` is the heap index         
  - `qp[i]` is `-1` if user didn't associate `i` with any element
### Problem
- The issue is that every time we have to swap in sink/swim we are having to swap the elements in the `keys` array too as `pq` and `keys` are parallel arrays. Especially bad when each key is large.

## Second approach
### Implementation
- `pq[i]`   => `i` is the heap index and `pq[i]` is the user specified index
- Essentially `pq` is the heap array where in each index we are storing the user index of the element instead of the actual element itself.
- `qp[i]`   => `i` is the user specified index and `qp[i]` is the heap index
  - `qp[i]` is -1 if user didn't associate `i` with any element
- `keys[i]` => `i` is the user specified index and `keys[i]` is the actual element
- qp and keys are parallel arrays where `i` is the user specified index
  - `qp[i]` holds the heap index and
  - `keys[i]` holds the corresponding item
- Note that if the element at a user index `i` was removed from the `pq`, `keys[i]` need not necessarily be empty. Since `qp[i]` is going to be `-1`, `keys[i]` is going to be irrelevant.

### Thoughts
- This approach solves the issue of the first approach where we had to swap the elements of the keys array too.