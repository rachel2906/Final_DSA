#include "ExternalMergeSort.h"
#include <fstream>
#include <queue>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <stdexcept>

static std::string makeChunkName(int id) {
    return "chunk_" + std::to_string(id) + ".tmp";
}

// Buffered write (binary)
static void writeChunk(const std::string& name, const std::vector<int>& buf, CostModel& cost) {
    std::ofstream out(name, std::ios::binary);
    if(!out) throw std::runtime_error("Cannot open file for writing: " + name);
    out.write(reinterpret_cast<const char*>(buf.data()), buf.size() * sizeof(int));
    cost.disk_write += buf.size();
}

// Buffered read (binary)
static bool readNext(std::ifstream& in, int& val, CostModel& cost) {
    if(in.read(reinterpret_cast<char*>(&val), sizeof(int))) {
        cost.disk_read++;
        return true;
    }
    return false;
}

// Sort một chunk (dùng thread)
static void sortChunk(std::vector<int>& buf, SortAlgorithm& sorter, CostModel& localCost) {
    sorter.sort(buf, localCost);
}

// ---------------- Split Input File into Sorted Chunks ----------------
void ExternalMergeSort::splitToChunks(
    const std::string& inputFile,
    size_t chunkSize,
    SortAlgorithm& sorter,
    std::vector<std::string>& chunks,
    CostModel& cost
) {
    std::ifstream in(inputFile, std::ios::binary);
    if(!in) throw std::runtime_error("Cannot open input file: " + inputFile);

    std::vector<int> buf;
    buf.reserve(chunkSize);

    int x;
    int id = 0;
    std::vector<std::thread> threads;
    std::mutex chunks_mutex;

    while(readNext(in, x, cost)) {
        buf.push_back(x);

        if(buf.size() == chunkSize) {
            std::vector<int> chunkBuf = std::move(buf);
            buf.clear();
            buf.reserve(chunkSize);
            int chunkId = id++;

            threads.emplace_back([&sorter, &chunks_mutex, &chunks, chunkBuf = std::move(chunkBuf), chunkId, &cost]() mutable {
                CostModel localCost;
                sortChunk(chunkBuf, sorter, localCost);

                std::string name = makeChunkName(chunkId);
                writeChunk(name, chunkBuf, localCost);

                {
                    std::lock_guard<std::mutex> lock(chunks_mutex);
                    chunks.push_back(name);
                    // Cộng localCost vào main cost (thread-safe)
                    cost.cpu_compare += localCost.cpu_compare;
                    cost.cpu_move    += localCost.cpu_move;
                    cost.heap_op     += localCost.heap_op;
                    cost.disk_read   += localCost.disk_read;
                    cost.disk_write  += localCost.disk_write;
                }
            });
        }
    }

    // Last chunk
    if(!buf.empty()) {
        std::vector<int> chunkBuf = std::move(buf);
        int chunkId = id++;

        threads.emplace_back([&sorter, &chunks_mutex, &chunks, chunkBuf = std::move(chunkBuf), chunkId, &cost]() mutable {
            CostModel localCost;
            sortChunk(chunkBuf, sorter, localCost);

            std::string name = makeChunkName(chunkId);
            writeChunk(name, chunkBuf, localCost);

            {
                std::lock_guard<std::mutex> lock(chunks_mutex);
                chunks.push_back(name);
                cost.cpu_compare += localCost.cpu_compare;
                cost.cpu_move    += localCost.cpu_move;
                cost.heap_op     += localCost.heap_op;
                cost.disk_read   += localCost.disk_read;
                cost.disk_write  += localCost.disk_write;
            }
        });
    }

    // Join all threads
    for(auto& t : threads) t.join();
}

// ---------------- Merge Sorted Chunks into Output File ----------------
void ExternalMergeSort::mergeChunks(
    const std::string& outputFile,
    const std::vector<std::string>& chunks,
    CostModel& cost
) {
    struct Node { int value; size_t idx; };
    auto cmp = [&](const Node& a, const Node& b) {
        cost.cpu_compare++;
        return a.value > b.value;
    };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> pq(cmp);

    std::vector<std::ifstream> files(chunks.size());
    for(size_t i = 0; i < chunks.size(); i++) {
        files[i].open(chunks[i], std::ios::binary);
        if(!files[i]) throw std::runtime_error("Cannot open chunk file: " + chunks[i]);
        int val;
        if(readNext(files[i], val, cost)) {
            pq.push({val, i});
            cost.heap_op++;
        }
    }

    std::ofstream out(outputFile, std::ios::binary);
    if(!out) throw std::runtime_error("Cannot open output file: " + outputFile);

    while(!pq.empty()) {
        Node cur = pq.top(); pq.pop();
        cost.heap_op++;

        out.write(reinterpret_cast<const char*>(&cur.value), sizeof(int));
        cost.disk_write++;
        cost.merge_step++;

        int next;
        if(readNext(files[cur.idx], next, cost)) {
            pq.push({next, cur.idx});
            cost.heap_op++;
        }
    }
}
