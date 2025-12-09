#include "storage.hpp"
#include <fstream>
#include <cstring>

StorageDevice::StorageDevice(const std::string& path, size_t blockSize)
        : filepath(path), blockSize(blockSize) {}

bool StorageDevice::writeBlock(size_t blockIndex, const std::vector<uint8_t>& data) {
    if(data.size() > blockSize) return false;

    std::fstream file(filepath, std::ios::in | std::ios::out | std::ios::binary);
    if(!file) {
        std::ofstream createFile(filepath, std::ios::binary);
        createFile.close();
        file.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
        if(!file) return false;
    }

    std::vector<uint8_t> buffer(blockSize, 0);
    std::memcpy(buffer.data(), data.data(), data.size());

    file.seekp(blockIndex * blockSize);
    file.write(reinterpret_cast<char*>(buffer.data()), blockSize);
    file.close();
    return true;
}

std::vector<uint8_t> StorageDevice::readBlock(size_t blockIndex) {
    std::vector<uint8_t> buffer(blockSize, 0);
    std::ifstream file(filepath, std::ios::binary);
    if(!file) return buffer;

    file.seekg(blockIndex * blockSize);
    file.read(reinterpret_cast<char*>(buffer.data()), blockSize);
    file.close();
    return buffer;
}
