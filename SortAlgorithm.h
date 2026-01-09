#pragma once
#include <vector>
#include <string>

class SortAlgorithm {
public:
    virtual ~SortAlgorithm() = default;
    virtual void sort(std::vector<int>& data) = 0;
    virtual std::string name() const = 0;
};
