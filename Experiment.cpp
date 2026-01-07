#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <cstdio>

#include "ExternalMergeSort.h"
#include "Analyzer.h"
#include "SortAlgorithm.h"

// factory functions
extern SortAlgorithm* createHeapSort();
extern SortAlgorithm* createQuickSort();

// ================== Generate input file ==================
void generateInput(const std::string& file, size_t N) {
    std::ofstream out(file, std::ios::binary);
    std::mt19937 rng(123);

    for (size_t i = 0; i < N; ++i) {
        int val = rng();
        out.write(reinterpret_cast<const char*>(&val), sizeof(int));
    }
}

// ================== Cleanup temp chunks ==================
void cleanup(const std::vector<std::string>& chunks) {
    for (const auto& f : chunks)
        std::remove(f.c_str());
}

// ================== Run 1 experiment ==================
BenchmarkRow runSingle(
    SortAlgorithm& algo,
    size_t chunkSize,
    size_t N
) {
    CostModel cost;
    cost.reset();

    std::vector<std::string> chunks;

    auto start = std::chrono::high_resolution_clock::now();

    ExternalMergeSort::splitToChunks(
        "input.txt",
        chunkSize,
        algo,
        chunks,
        cost
    );

    ExternalMergeSort::mergeChunks(
        "output.txt",
        chunks,
        cost
    );

    auto end = std::chrono::high_resolution_clock::now();

    cleanup(chunks);

    return {
        algo.name(),
        chunkSize,
        N,
        cost.cpu_cost(),
        cost.io_cost(),
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    };
}

// ================== Benchmark controller ==================
void runBenchmark() {
    const size_t N = 2'000'000; // 2 triệu số
    generateInput("input.txt", N);

    std::vector<size_t> chunkSizes = {1000, 5000, 20000};
    std::vector<BenchmarkRow> rows;

    std::vector<SortAlgorithm*(*)()> algos = {
        createHeapSort,
        createQuickSort
    };

    for (size_t chunk : chunkSizes) {
        for (auto createAlgo : algos) {
            std::unique_ptr<SortAlgorithm> algo(createAlgo());

            std::cout << "DEBUG: Running "
                      << algo->name()
                      << " with chunk size "
                      << chunk << std::endl;

            rows.push_back(runSingle(*algo, chunk, N));
        }
    }

    std::cout << "\n";
    printTable(rows);

    std::cout << "\n=== Notes ===\n";
    std::cout << "CPU cost = comparisons + moves + heap ops\n";
    std::cout << "IO cost  = disk read + disk write\n";
}

// ================== MAIN ==================
int main() {
    std::cout << "=== High-Performance Big Data File Sorter ===\n\n";
    runBenchmark();
    return 0;
}
