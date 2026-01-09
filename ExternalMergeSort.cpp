#include "ExternalMergeSort.h"
#include "HeapSort.h"
#include <fstream>
#include <queue>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ExternalMergeSort::ExternalMergeSort(size_t chunkSize) : chunkSize(chunkSize) {}

std::vector<std::string> ExternalMergeSort::splitAndSortChunks(const std::string& inputFile) {
    std::ifstream in(inputFile);
    std::vector<std::string> chunkFiles;
    auto heapSort = std::make_unique<HeapSort>();

    if (!in.is_open()) {
        std::cerr << "[ERROR] Khong the mo file input: " << inputFile << std::endl;
        return {};
    }

    int value;
    while (in.peek() != EOF) {
        std::vector<int> buffer;
        buffer.reserve(chunkSize);

        while (buffer.size() < chunkSize && (in >> value)) {
            buffer.push_back(value);
        }

        if (buffer.empty()) break;

        // Sap xep du lieu trong RAM
        heapSort->sort(buffer);

        // Tao file tam
        std::string filename = "chunk_" + std::to_string(chunkFiles.size()) + ".tmp";
        std::ofstream out(filename);
        if (out.is_open()) {
            for (const int v : buffer) {
                out << v << "\n";
            }
            out.flush(); // Ep du lieu xuong dia
            out.close(); // DONG FILE NGAY de Phase 2 co the doc duoc
            
            std::cout << "[Phase 1] Da tao va sap xep file tam: " << filename << std::endl;
            chunkFiles.push_back(filename);
        }
    }
    in.close();
    return chunkFiles;
}

void ExternalMergeSort::mergeChunks(const std::vector<std::string>& chunkFiles, const std::string& outputFile) {
    if (chunkFiles.empty()) return;

    struct Node {
        int value;
        size_t fileIndex;
        bool operator>(const Node& other) const { return value > other.value; }
    };

    std::vector<std::ifstream> inputs;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> minHeap;

    std::cout << "[Phase 2] Dang chuan bi gop " << chunkFiles.size() << " file tam..." << std::endl;

    // Mo tat ca cac file tam va lay phan tu dau tien
    for (size_t i = 0; i < chunkFiles.size(); ++i) {
        inputs.emplace_back(chunkFiles[i]);
        int val;
        if (inputs[i] >> val) {
            minHeap.push({val, i});
        }
    }

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "[ERROR] Khong the ghi file output: " << outputFile << std::endl;
        return;
    }

    int count = 0;
    while (!minHeap.empty()) {
        Node current = minHeap.top();
        minHeap.pop();
        
        out << current.value << "\n";
        count++;

        int nextVal;
        if (inputs[current.fileIndex] >> nextVal) {
            minHeap.push({nextVal, current.fileIndex});
        }
    }
    
    out.flush();
    out.close();
    std::cout << "[Phase 2] Da gop xong " << count << " phan tu vao: " << outputFile << std::endl;

    // Dong toan bo streams truoc khi xoa
    for (auto& f : inputs) f.close();

    // Don dep file tam
    std::cout << "[Final] Dang don dep tai nguyen..." << std::endl;
    for (const auto& name : chunkFiles) {
        if (fs::exists(name)) {
            fs::remove(name);
            std::cout << "  - Da xoa file tam: " << name << std::endl;
        }
    }
}

void ExternalMergeSort::sortFile(const std::string& inputFile, const std::string& outputFile) {
    auto chunks = splitAndSortChunks(inputFile);
    if (!chunks.empty()) {
        mergeChunks(chunks, outputFile);
    } else {
        std::cout << "[Warning] Khong co du lieu de gop (File dau vao co the bi trong)." << std::endl;
    }
}
