#include "cppelf/utils/MemBlock.h"
#include <cstring>

namespace cppelf::utils {

MemBlock::~MemBlock() {
    if (owning_)
        delete[] data_;
}

std::unique_ptr<MemBlock> MemBlock::createOwning(const char *data,
                                                 size_t size) {
    // Allocate block
    size_t totalSize = size + sizeof(MemBlock);
    void *block = ::operator new(totalSize);

    // Copy payload
    std::memcpy(static_cast<char *>(block) + sizeof(MemBlock), data, size);

    // Construct MemBlock
    MemBlock *memBlock = new (block) MemBlock(data, true, size);
    return std::unique_ptr<MemBlock>(memBlock);
}

MemBlock MemBlock::createNonOwning(const char *data, size_t size) {
    return MemBlock(data, false, size);
}

MemBlock::MemBlock(const char *data, bool owning, size_t size)
    : size_(size), owning_(owning), data_(data) {}

} // namespace cppelf::utils