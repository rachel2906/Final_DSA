#pragma once
#include <string>
#include <sstream>

struct CostModel {

    long long cpu_compare = 0;
    long long cpu_move    = 0;
    long long heap_op     = 0;

    long long disk_read  = 0;
    long long disk_write = 0;

    long long merge_step = 0;

    void reset() {
        cpu_compare = cpu_move = heap_op = 0;
        disk_read = disk_write = 0;
        merge_step = 0;
    }

    long long cpu_cost() const {
        return cpu_compare + cpu_move + heap_op;
    }

    long long io_cost() const {
        return disk_read + disk_write;
    }

    std::string dominant() const {
        return (io_cost() > cpu_cost()) ? "I/O-bound" : "CPU-bound";
    }

    std::string summary() const {
        std::ostringstream oss;
        oss << "CPU=" << cpu_cost()
            << ", IO=" << io_cost()
            << ", Merge=" << merge_step
            << ", Bottleneck=" << dominant();
        return oss.str();
    }
};
