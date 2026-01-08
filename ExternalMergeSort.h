#pragma once
#include <vector>
#include <string>

class ExternalMergeSort {
public:
    // Constructor với chunk size
    ExternalMergeSort(size_t chunkSize);

    // Public API: sort file input -> output
    void sortFile(const std::string& inputFile,
                  const std::string& outputFile);

private:
    size_t chunkSize;

    // Chia file lớn thành chunk nhỏ và sort trong RAM
    std::vector<std::string> splitAndSortChunks(const std::string& inputFile);

    // Merge các chunk đã sort
    void mergeChunks(const std::vector<std::string>& chunks,
                     const std::string& outputFile);
};
