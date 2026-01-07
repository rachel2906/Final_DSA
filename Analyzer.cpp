#include "Analyzer.h"
#include <iostream>
#include <iomanip>

void printTable(const std::vector<BenchmarkRow>& rows) {
    std::cout << std::left
        << std::setw(20) << "Algorithm"
        << std::setw(10) << "Chunk"
        << std::setw(10) << "N"
        << std::setw(15) << "CPU"
        << std::setw(15) << "IO"
        << std::setw(10) << "Time(ms)\n";

    std::cout << std::string(80, '-') << "\n";

    for (const auto& r : rows) {
        std::cout
            << std::setw(20) << r.algorithm
            << std::setw(10) << r.chunkSize
            << std::setw(10) << r.dataSize
            << std::setw(15) << r.cpuCost
            << std::setw(15) << r.ioCost
            << std::setw(10) << r.timeMs << "\n";
    }
}
