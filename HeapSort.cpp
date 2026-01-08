#include "HeapSort.h"
#include <algorithm>

void HeapSort::sort(std::vector<int>& data) {
    // Build max heap
    std::make_heap(data.begin(), data.end());

    // Pop elements one by one to sort
    for (auto it = data.end(); it != data.begin(); --it) {
        std::pop_heap(data.begin(), it);
    }
}
