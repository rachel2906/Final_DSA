#include "HeapSort.h"
#include <algorithm> // std::swap

void HeapSort::heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;        // giả sử node hiện tại là lớn nhất
    int left = 2 * i + 1;   // con trái
    int right = 2 * i + 2;  // con phải

    // nếu con trái lớn hơn cha
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // nếu con phải lớn hơn cha hiện tại
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // nếu cha không còn là lớn nhất
    if (largest != i) {
        std::swap(arr[i], arr[largest]);

        // tiếp tục sửa cây con bị ảnh hưởng
        heapify(arr, n, largest);
    }
}


