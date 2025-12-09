using namespace std;
#include "storage.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <chrono>
#include <string>
#include <fstream>

class LRUCache {
public:
    LRUCache(size_t capacity) : capacity(capacity) {}

    bool get(size_t blockId, vector<uint8_t>& out) {
        auto it = map.find(blockId);
        if(it == map.end()) return false;
        lru.splice(lru.begin(), lru, it->second.second);
        out = it->second.first;
        return true;
    }

    void put(size_t blockId, const vector<uint8_t>& data) {
        auto it = map.find(blockId);
        if(it != map.end()) {
            it->second.first = data;
            lru.splice(lru.begin(), lru, it->second.second);
            return;
        }
        if(map.size() >= capacity) {
            size_t old = lru.back();
            lru.pop_back();
            map.erase(old);
        }
        lru.push_front(blockId);
        map[blockId] = {data, lru.begin()};
    }

private:
    size_t capacity;
    list<size_t> lru;
    unordered_map<size_t, pair<vector<uint8_t>, list<size_t>::iterator>> map;
};

struct Stats {
    size_t reads = 0;
    size_t writes = 0;
    size_t cacheHits = 0;
    size_t cacheMisses = 0;
    double totalReadMs = 0;
    double totalWriteMs = 0;
};

int main() {
    const string diskFile = "disk.bin";
    const size_t totalBlocks = 2048;
    const size_t blockSize = 512;

    ifstream infile(diskFile, ios::binary);
    if(!infile.good()) {
        ofstream outfile(diskFile, ios::binary);
        vector<char> zeros(blockSize, 0);
        for(size_t i = 0; i < totalBlocks; ++i)
            outfile.write(zeros.data(), blockSize);
        outfile.close();
        cout << "Created new disk file: " << diskFile << endl;
    }
    infile.close();

    StorageDevice dev(diskFile, blockSize);
    LRUCache cache(16);
    Stats stats;

    cout << "Minimal Storage Simulator (C++)\n";
    cout << "Commands: read <block>, write <block> <text>, stats, exit\n";

    string cmd;
    while(true) {
        cout << ">";
        if(!(cin >> cmd)) break;

        if(cmd == "exit") break;

        if(cmd == "stats") {
            cout << "reads: " << stats.reads << ", writes: " << stats.writes << "\n";
            cout << "cache hits: " << stats.cacheHits << ", cache misses: " << stats.cacheMisses << "\n";
            cout << "avg read ms: " << (stats.reads ? stats.totalReadMs / stats.reads : 0) << "\n";
            cout << "avg write ms: " << (stats.writes ? stats.totalWriteMs / stats.writes : 0) << "\n";
            continue;
        }

        if(cmd == "read") {
            size_t block; cin >> block;
            vector<uint8_t> data;
            auto t0 = std::chrono::high_resolution_clock::now();
            if(cache.get(block, data)) stats.cacheHits++;
            else {
                stats.cacheMisses++;
                data = dev.readBlock(block);
                cache.put(block, data);
            }
            auto t1 = std::chrono::high_resolution_clock::now();
            stats.reads++;
            stats.totalReadMs += std::chrono::duration<double, milli>(t1-t0).count();

            string s;
            for(auto b : data) if(b != 0) s.push_back((char)b);
            cout << "[block " << block << "] data: '" << s << "'\n";
            continue;
        }

        if(cmd == "write") {
            size_t block; string payload;
            cin >> block;
            getline(cin, payload);
            if(payload.size() && payload[0]==' ') payload.erase(0,1);

            vector<uint8_t> data(blockSize, 0);
            for(size_t i = 0; i < payload.size() && i < blockSize; ++i)
                data[i] = (uint8_t)payload[i];

            auto t0 = std::chrono::high_resolution_clock::now();
            dev.writeBlock(block, data);
            cache.put(block, data);
            auto t1 = std::chrono::high_resolution_clock::now();

            stats.writes++;
            stats.totalWriteMs += std::chrono::duration<double, milli>(t1-t0).count();
            cout << "written\n";
            continue;
        }

        cout << "unknown command\n";
    }

    cout << "Exiting. Final stats:\n";
    cout << "reads: " << stats.reads << ", writes: " << stats.writes << "\n";
    cout << "cache hits: " << stats.cacheHits << ", cache misses: " << stats.cacheMisses << "\n";

    return 0;
}
