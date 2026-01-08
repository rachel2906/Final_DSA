#pragma once
#include "SortAlgorithm.h"

class HeapSort : public SortAlgorithm {
public:
    void sort(std::vector<int>& data) override;
    std::string name() const override { return "HeapSort"; }
};
