#include "Analyzer.h"
#include "ExternalMergeSort.h"
#include <iostream>

int main() {
    Analyzer::explain();

    ExternalMergeSort sorter(100000);
    
    std::string input = "../data/input.txt";
    std::string output = "../data/output.txt";

    std::cout << "Processing Big Data File...\n";
    sorter.sortFile(input, output);
    std::cout << "Sorting completed successfully.\n";

    return 0;
}
