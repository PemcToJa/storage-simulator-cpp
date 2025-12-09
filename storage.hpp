#pragma once
#include <vector>
#include <string>
#include <cstdint>

class StorageDevice {
public:
    StorageDevice(const std::string& path, size_t blockSize);

    bool writeBlock(size_t blockIndex, const std::vector<uint8_t>& data);
    std::vector<uint8_t> readBlock(size_t blockIndex);

private:
    std::string filepath;
    size_t blockSize;
};
