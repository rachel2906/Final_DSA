#include "SortAlgorithm.h"
#include <algorithm>

class QuickSort : public SortAlgorithm {
    void qsort(std::vector<int>& a, int l, int r, CostModel& cost) {
        if (l >= r) return;

        int pivot = a[r];
        int i = l - 1;

        for (int j = l; j < r; j++) {
            cost.cpu_compare++;
            if (a[j] < pivot) {
                ++i;
                std::swap(a[i], a[j]);
                cost.cpu_move++;
            }
        }

        std::swap(a[i + 1], a[r]);
        cost.cpu_move++;

        int p = i + 1;
        qsort(a, l, p - 1, cost);
        qsort(a, p + 1, r, cost);
    }

public:
    void sort(std::vector<int>& a, CostModel& cost) override {
        if (!a.empty())
            qsort(a, 0, (int)a.size() - 1, cost);
    }

    const char* name() const override {
        return "QuickSort";
    }
};

SortAlgorithm* createQuickSort() {
    return new QuickSort();
}
