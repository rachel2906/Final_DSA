#include "Analyzer.h"
#include <iostream>
#include <iomanip>

void Analyzer::explain() {
    std::cout << "\n" << std::setfill('=') << std::setw(50) << "" << std::endl;
    std::cout << "RESEARCH TOPIC: EXTERNAL MERGE SORT ANALYSIS" << std::endl;
    std::cout << std::setfill('=') << std::setw(50) << "" << std::endl;

    std::cout << "1. Algorithmic Foundation:\n"
              << "   - Reference: Donald Knuth's TAOCP Vol. 3 (Sorting and Searching).\n"
              << "   - Approach: 2-Phase Multiway Merge Sort.\n"
              << "   - Complexity: O(N * log_k(N/M)), where k is the merge order.\n\n";

    std::cout << "2. Memory & Performance Trade-offs:\n"
              << "   - HeapSort vs QuickSort: HeapSort provides a guaranteed O(n log n)\n"
              << "     worst-case, making chunking time predictable.\n"
              << "   - Modern C++: Utilizes std::priority_queue (Min-Heap) for K-way merge,\n"
              << "     minimizing disk seek operations.\n\n";

    std::cout << "3. Modern Standards Applied:\n"
              << "   - RAII: Automatic resource management for file streams.\n"
              << "   - C++17 Filesystem: Secure handling of temporary chunk files.\n"
              << "   - Smart Pointers: Ensuring zero memory leaks.\n";
    std::cout << std::setfill('-') << std::setw(50) << "" << std::endl << std::endl;
}
