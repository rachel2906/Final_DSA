#include "HeapSort.h"
#include <algorithm>

void HeapSort::sort(std::vector<int>& data) {
    // Phase 1: Biến vector thành Max-Heap (O(n))
    std::make_heap(data.begin(), data.end());

    // Phase 2: Sắp xếp Heap thành mảng tăng dần (O(n log n))
    std::sort_heap(data.begin(), data.end());
}
