#pragma once
#include <vector>
#include <string>

class SortAlgorithm {
public:
    virtual ~SortAlgorithm() = default;

    // Sort data in memory
    virtual void sort(std::vector<int>& data) = 0;

    // Name of algorithm (for analysis & explanation)
    virtual std::string name() const = 0;
};
