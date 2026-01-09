#pragma once
#include "SortAlgorithm.h"

class QuickSort : public SortAlgorithm {
public:
    void sort(std::vector<int>& data) override;
    std::string name() const override { return "QuickSort"; }

private:
    private:
    void quicksort(std::vector<int>& data, int left, int right); 
};
