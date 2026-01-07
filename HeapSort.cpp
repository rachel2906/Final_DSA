#include "SortAlgorithm.h"
#include <algorithm>

class HeapSort : public SortAlgorithm {
public:
    void sort(std::vector<int>& a, CostModel& cost) override {
        std::make_heap(a.begin(), a.end(), [&](int x, int y) {
            cost.cpu_compare++;
            return x > y;
        });
        cost.heap_op += a.size();

        for (int i = (int)a.size(); i > 1; --i) {
            std::pop_heap(a.begin(), a.begin() + i);
            cost.heap_op++;
            std::swap(a[0], a[i - 1]);
            cost.cpu_move++;
        }
    }

    const char* name() const override {
        return "HeapSort (Chunk)";
    }
};

SortAlgorithm* createHeapSort() {
    return new HeapSort();
}
