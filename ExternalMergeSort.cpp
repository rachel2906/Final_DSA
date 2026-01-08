#include "ExternalMergeSort.h"
#include "HeapSort.h"

#include <fstream>
#include <queue>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio> 

ExternalMergeSort::ExternalMergeSort(size_t chunkSize)
    : chunkSize(chunkSize) {}

/*
    Phase 1: Split input file into sorted chunks
*/
std::vector<std::string> ExternalMergeSort::splitAndSortChunks(const std::string& inputFile) {
    std::ifstream in(inputFile);
    std::vector<std::string> chunkFiles;
    HeapSort heapSort;

    if (!in.is_open()) {
        std::cerr << "Error: Cannot open input file " << inputFile << "\n";
        return {};
    }

    int x;
    while (true) {
        std::vector<int> buffer;

        // đọc tối đa chunkSize phần tử
        while (buffer.size() < chunkSize && (in >> x)) {
            buffer.push_back(x);
        }

        if (buffer.empty()) break;

        // sort chunk trong RAM
        heapSort.sort(buffer);

        // ghi chunk ra file tạm
        std::string filename = "chunk_" + std::to_string(chunkFiles.size()) + ".tmp";
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cerr << "Error: Cannot write chunk file " << filename << "\n";
            break;
        }

        for (int v : buffer) out << v << "\n";

        chunkFiles.push_back(filename);
    }

    return chunkFiles;
}

/*
    Phase 2: Merge k sorted chunks
*/
void ExternalMergeSort::mergeChunks(
    const std::vector<std::string>& chunkFiles,
    const std::string& outputFile) {

    if (chunkFiles.empty()) {
        std::cerr << "Warning: No chunks to merge.\n";
        return;
    }

    struct Node {
        int value;
        size_t fileIndex;
        bool operator>(const Node& other) const { return value > other.value; }
    };

    // mở tất cả các chunk
    std::vector<std::ifstream> inputs(chunkFiles.size());
    for (size_t i = 0; i < chunkFiles.size(); ++i) {
        inputs[i].open(chunkFiles[i]);
        if (!inputs[i].is_open()) {
            std::cerr << "Error: Cannot open chunk file " << chunkFiles[i] << "\n";
            return;
        }
    }

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> minHeap;

    // đọc phần tử đầu tiên của từng chunk
    for (size_t i = 0; i < inputs.size(); ++i) {
        int x;
        if (inputs[i] >> x)
            minHeap.push({x, i});
    }

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Error: Cannot write output file " << outputFile << "\n";
        return;
    }

    // merge k-way
    while (!minHeap.empty()) {
        Node cur = minHeap.top(); minHeap.pop();
        out << cur.value << "\n";

        if (cur.fileIndex >= inputs.size()) continue; 
        int next;
        if (inputs[cur.fileIndex].good() && (inputs[cur.fileIndex] >> next))
            minHeap.push({next, cur.fileIndex});
    }

    // đóng tất cả file chunk
    for (auto& f : inputs) f.close();

    // xóa các file chunk tạm
    for (auto& name : chunkFiles) {
        if (std::remove(name.c_str()) != 0)
            std::cerr << "Warning: Cannot delete temporary file " << name << "\n";
    }
}

/*
    Public API
*/
void ExternalMergeSort::sortFile(const std::string& inputFile,
                                 const std::string& outputFile) {
    auto chunks = splitAndSortChunks(inputFile);
    mergeChunks(chunks, outputFile);
}
