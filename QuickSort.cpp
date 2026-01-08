#include "QuickSort.h"
#include <algorithm>

void QuickSort::sort(std::vector<int>& data) {
    if (!data.empty())
        quicksort(data, 0, (int)data.size() - 1);
}

void QuickSort::quicksort(std::vector<int>& data, int left, int right) {
    int i = left, j = right;
    int pivot = data[(left + right) / 2];

    while (i <= j) {
        while (data[i] < pivot) i++;
        while (data[j] > pivot) j--;
        if (i <= j) {
            std::swap(data[i], data[j]);
            i++; j--;
        }
    }

    if (left < j) quicksort(data, left, j);
    if (i < right) quicksort(data, i, right);
}
