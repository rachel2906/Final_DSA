#include "Analyzer.h"
#include "ExternalMergeSort.h"
#include <iostream>
#include <fstream>
#include <random>
#include <string>

void createData(const std::string& path, int n, int type) {
    std::ofstream out(path);
    if (!out) return;

    if (type == 1) { 
        std::random_device rd; std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 1000000);
        for (int i = 0; i < n; ++i) out << dis(gen) << "\n";
    } 
    else if (type == 2) { 
        for (int i = 0; i < n; ++i) out << i << "\n";
    } 
    else if (type == 3) { 
        for (int i = n; i > 0; --i) out << i << "\n";
    }
    out.close();
}

int main() {
    Analyzer::explain();
    int size, chunk, type;

    std::cout << "\n[DEMO MODE]\n";
    std::cout << "1. Random | 2. Sorted | 3. Reverse: ";
    std::cin >> type;
    std::cout << "Nhap so luong phan tu: ";
    std::cin >> size;
    std::cout << "Nhap chunkSize: ";
    std::cin >> chunk;

    createData("../data/input.txt", size, type); 

    ExternalMergeSort sorter(chunk);
    std::cout << "\nProcessing...\n";
    sorter.sortFile("../data/input.txt", "../data/output.txt");

    std::cout << "Sorting completed successfully.\n";
    return 0;
}
