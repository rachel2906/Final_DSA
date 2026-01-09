#pragma once
#include <vector>
#include <string>
#include <memory>
#include <filesystem>

class ExternalMergeSort {
public:
    // Sử dụng size_t cho chunkSize (Byte hoặc số lượng phần tử)
    explicit ExternalMergeSort(size_t chunkSize);

    void sortFile(const std::string& inputFile, const std::string& outputFile);

private:
    size_t chunkSize;
    // Sử dụng std::filesystem để quản lý file hiện đại
    std::vector<std::string> splitAndSortChunks(const std::string& inputFile);
    void mergeChunks(const std::vector<std::string>& chunkFiles, const std::string& outputFile);
};
