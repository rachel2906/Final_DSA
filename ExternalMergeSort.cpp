#include "ExternalMergeSort.h"
#include "HeapSort.h"
#include <fstream>
#include <queue>
#include <iostream>

namespace fs = std::filesystem;

ExternalMergeSort::ExternalMergeSort(size_t chunkSize) : chunkSize(chunkSize) {}

std::vector<std::string> ExternalMergeSort::splitAndSortChunks(const std::string& inputFile) {
    std::ifstream in(inputFile);
    std::vector<std::string> chunkFiles;
    auto heapSort = std::make_unique<HeapSort>(); // Modern Memory Management

    if (!in.is_open()) return {};

    int value;
    while (in.peek() != EOF) {
        std::vector<int> buffer;
        buffer.reserve(chunkSize); // Tối ưu bộ nhớ, tránh reallocation

        while (buffer.size() < chunkSize && (in >> value)) {
            buffer.push_back(value);
        }

        if (buffer.empty()) break;

        heapSort->sort(buffer);

        std::string filename = "chunk_" + std::to_string(chunkFiles.size()) + ".tmp";
        std::ofstream out(filename);
        for (const int v : buffer) out << v << "\n";
        
        chunkFiles.push_back(filename);
    }
    return chunkFiles;
}

void ExternalMergeSort::mergeChunks(const std::vector<std::string>& chunkFiles, const std::string& outputFile) {
    struct Node {
        int value;
        size_t fileIndex;
        bool operator>(const Node& other) const { return value > other.value; }
    };

    std::vector<std::ifstream> inputs;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> minHeap;

    for (size_t i = 0; i < chunkFiles.size(); ++i) {
        inputs.emplace_back(chunkFiles[i]);
        int val;
        if (inputs[i] >> val) {
            minHeap.push({val, i});
        }
    }

    std::ofstream out(outputFile);
    while (!minHeap.empty()) {
        Node current = minHeap.top();
        minHeap.pop();
        out << current.value << "\n";

        int nextVal;
        if (inputs[current.fileIndex] >> nextVal) {
            minHeap.push({nextVal, current.fileIndex});
        }
    }

    // Đóng toàn bộ streams trước khi xóa
    for (auto& f : inputs) f.close();

    // Modern C++: Sử dụng std::filesystem để dọn dẹp file tạm
    for (const auto& name : chunkFiles) {
        fs::remove(name);
    }
}

void ExternalMergeSort::sortFile(const std::string& inputFile, const std::string& outputFile) {
    auto chunks = splitAndSortChunks(inputFile);
    mergeChunks(chunks, outputFile);
}
