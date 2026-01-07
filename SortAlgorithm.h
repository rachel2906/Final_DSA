#pragma once
#include <vector>
#include "CostModel.h"

class SortAlgorithm {
public:
    virtual ~SortAlgorithm() = default;
    virtual void sort(std::vector<int>& data, CostModel& cost) = 0;
    virtual const char* name() const = 0;
};
