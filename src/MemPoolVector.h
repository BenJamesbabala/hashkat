/* Defines a pooled-memory integer vector and its grower.
 * This class is necessary for implementing follow sets.
 *
 * Follow sets are lists of (hopefully, but in practice not always)
 * unique entities that a given entity is following.
 *
 * We optimize for the case size <= MEMPOOL_THRESHOLD by placing the
 * elements directly in the array.
 *
 * For size > MEMPOOL_THRESHOLD, MemPoolVectorGrower provides a new follow
 * set to point to, when size == MEMPOOL_THRESHOLD, and grow() is used.
 *
 */

#ifndef MEMPOOLVECTOR_H_
#define MEMPOOLVECTOR_H_

#include <vector>
#include <map>
#include <algorithm>

#include "util.h"
#include "config_static.h"

// Above a certain threshold,
template <typename V, int MEMPOOL_THRESHOLD>
struct MemPoolVector {
    MemPoolVector() {
        size = 0;
        location = buffer1;
        capacity = MEMPOOL_THRESHOLD;
        copy(NULL, 0);
    }

    void copy(V* source, int len) {
        for (int i = 0; i < len; i++) {
        	location[i] = source[i];
        }
        for (int i = len; i < capacity; i++){
            location[i] = -1; // Fill rest with -1 to show no action in the array
        }
    }

    void sort_and_remove_duplicates() {
    	std::sort(location, location + size);
    	V* new_end = std::unique(location, location + size);
    	size = (new_end - location); // Where we start minus the new end
    }

    int& operator[](int index) { //** This allows us to index our MemPoolVector struct as if it were an array.
    	DEBUG_CHECK(within_range(index, 0, size), "Operator out of bounds");
    	return location[index];
    }

    int size, capacity;
    V* location;
	// Define buffer1 as an array of fixed-length 'MEMPOOL_THRESHOLD'
    // When size <= MEMPOOL_THRESHOLD, location points to this array
    V buffer1[MEMPOOL_THRESHOLD]; // (Short-object-optimization)
};

struct DeletedMemPoolVector {
	// A buffer in our pool that overgrew its boundary, and can be used by another entity.
    void* location;
	int capacity;
	DeletedMemPoolVector(void* loc, int cap) {
		location = loc, capacity = cap;
	}
};


/* Handles growing follow sets, and allocating buffers when size > MEMPOOL_THRESHOLD. */
struct MemPoolVectorGrower {
    typedef std::vector< DeletedMemPoolVector > DeletedList;

	void* memory;
	int used, capacity;
	DeletedList deletions;

	MemPoolVectorGrower() {
		memory = NULL;
		used = 0, capacity = 0;
	}
    void preallocate(int max_bytes) {
        capacity = max_bytes;
        memory = (char*)malloc(capacity);
        printf("%d bytes in preallocate\n", max_bytes);
        ASSERT(memory != NULL, "MemPoolVectorGrower::preallocate failed")
    }
	~MemPoolVectorGrower() {
		free(memory);
	}

	// Add an element to the pooled vector, potentially growing (reallocating) the array.
	// If we had to grow AND we have run out of allocated memory, we do nothing and return false.
	// Otherwise, we return true.
    template <typename V, int MEMPOOL_THRESHOLD>
	bool add_if_possible(MemPoolVector<V, MEMPOOL_THRESHOLD>& f, int element) {
    	DEBUG_CHECK(f.size <= f.capacity, "Logic error, array should have been grown!");
		if (f.size == f.capacity) {
			if (!grow_follow_set(f)) {
				// Not enough room for this MemPoolVector allocation. Do nothing here.
				return false;
			}
		}
    	DEBUG_CHECK(f.size < f.capacity, "Logic error");
		f.location[f.size] = element;
		f.size++;
		return true;
	}
private:
	template <typename V, int MEMPOOL_THRESHOLD>
	bool grow_follow_set(MemPoolVector<V, MEMPOOL_THRESHOLD>& f) {
		static std::map<void*, void*> allocs;

		const int GROWTH_MULTIPLE = 2;
		int new_capacity = f.capacity * sizeof(V) * GROWTH_MULTIPLE;
		ASSERT(new_capacity % 64 == 0, "MemPoolVector allocations must be 64-byte aligned!");
		V* new_location = NULL;
		// Search recycled buffers
		typename DeletedList::iterator candidate = deletions.begin();
		for (; candidate != deletions.end(); ++candidate) {
			// Compatible deleted buffer, recycle it:
			if (candidate->capacity == new_capacity) {
				new_location = (V*)candidate->location;
				deletions.erase(candidate);
				break;
			}
		}

		// Go into our block of memory and allocate a pooled vector:
		if (new_location == NULL) {
			new_location = allocate<V>(new_capacity);
		}
		if (new_location == NULL) {
			return false; // Not enough room!
		}

		// Recycle our old buffer
		if (f.location != f.buffer1) {
			// If we are not pointing to our embedded 'buffer1', we must be pointing within the 'memory' array
			DEBUG_CHECK(within_range((char*)f.location, (char*)memory, (char*)memory + capacity), "Logic error");
			deletions.push_back(DeletedMemPoolVector(f.location, f.capacity));
		}
		V* old_loc = f.location;
		f.location = new_location;
		f.capacity = new_capacity / sizeof(V);
		f.copy(old_loc, f.size);
		return true;
	}

	template <typename V>
	V* allocate(int cap) {
		if (used + cap > capacity) {
			return NULL; // Not enough room!
		}
		used += cap;
		return (V*)&((char*)memory)[used - cap];
	}
};

#endif
