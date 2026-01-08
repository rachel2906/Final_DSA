#include "Analyzer.h"
#include "ExternalMergeSort.h"
#include <iostream>
#include <fstream>
#include <chrono>   
#include <random>   

void generateRandomFile(const std::string& filename, size_t count) {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    std::cout << "--- Generating " << count << " random numbers into " << filename << "... ---\n";
    for (size_t i = 0; i < count; ++i) {
        out << dis(gen) << "\n";
    }
    out.close();
    std::cout << "--- Generation completed. ---\n\n";
}

int main() {
   
    std::string inputPath = "../data/input.txt";
    std::string outputPath = "../data/output.txt";
    
    std::cout << "====================================================\n";
    std::cout << "=== High-Performance Big Data File Sorter ===\n";
    std::cout << "====================================================\n";

    // Phân tích thuật toán
    Analyzer::explain();

    // Kiểm tra file đầu vào, nếu chưa có thì tạo mới 500,000 số để test
    std::ifstream checkFile(inputPath);
    if (!checkFile.good()) {
        generateRandomFile(inputPath, 500000); 
    } else {
        checkFile.close();
    }

    // Khởi tạo bộ sắp xếp với Chunk Size 
    ExternalMergeSort sorter(100000);

    std::cout << "Sorting started...\n";
    
    // Bắt đầu đo thời gian
    auto start = std::chrono::high_resolution_clock::now();

    // Thực hiện sắp xếp
    sorter.sortFile(inputPath, outputPath);

    // Kết thúc đo thời gian
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    // 5. Thông báo kết quả
    std::cout << "----------------------------------------------------\n";
    std::cout << "SUCCESS: Sorting completed!\n";
    std::cout << "Input:  " << inputPath << "\n";
    std::cout << "Output: " << outputPath << "\n";
    std::cout << "Time taken: " << diff.count() << " seconds\n";
    std::cout << "----------------------------------------------------\n";

    return 0;
}