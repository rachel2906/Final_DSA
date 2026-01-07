#pragma once
#include <vector>
#include <string>

struct BenchmarkRow {
    std::string algorithm;
    size_t chunkSize;
    size_t dataSize;
    long long cpuCost;
    long long ioCost;
    long long timeMs;
};

void printTable(const std::vector<BenchmarkRow>& rows);
