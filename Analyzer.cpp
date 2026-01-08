#include "Analyzer.h"

void Analyzer::explain() {
    std::cout << "\n--- Algorithm Analysis ---\n";

    std::cout << "QuickSort:\n";
    std::cout << "- Average: O(n log n)\n";
    std::cout << "- Worst-case: O(n^2)\n";
    std::cout << "- Recursive, not ideal for external sorting\n\n";

    std::cout << "HeapSort:\n";
    std::cout << "- Guaranteed O(n log n)\n";
    std::cout << "- Non-recursive, memory safe\n";
    std::cout << "- Preferred for chunk sorting\n\n";

    std::cout << "External Merge Sort:\n";
    std::cout << "- RAM usage limited by chunk size\n";
    std::cout << "- Sequential disk I/O\n";
    std::cout << "- Classical solution in Knuth Vol. 3\n";
}
