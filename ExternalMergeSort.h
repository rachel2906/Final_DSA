#pragma once
#include <string>
#include <vector>
#include "SortAlgorithm.h"
#include "CostModel.h"

class ExternalMergeSort {
public:
    // Chia input file thành các chunk, sort từng chunk, ghi ra file tạm
    static void splitToChunks(
        const std::string& inputFile,
        size_t chunkSize,
        SortAlgorithm& sorter,
        std::vector<std::string>& chunks,
        CostModel& cost
    );

    // Merge tất cả chunk thành output file
    static void mergeChunks(
        const std::string& outputFile,
        const std::vector<std::string>& chunks,
        CostModel& cost
    );
};
