#include "cppelf/utils/MemBlock.h"
#include <cstring>

namespace cppelf::utils {

MemBlock::~MemBlock() {
    if (owning_)
        delete[] data_;
}

MemBlock::MemBlock(MemBlock &&other) noexcept
    : size_(other.size_), owning_(other.size_), data_(other.data_) {
    other.size_ = 0;
    other.owning_ = false;
}

MemBlock &MemBlock::operator=(MemBlock &&other) noexcept {
    if (&other == this)
        return *this;
    if (owning_)
        delete[] data_;

    size_ = other.size_;
    owning_ = other.owning_;

    other.size_ = 0;
    other.owning_ = false;

    return *this;
}

MemBlock MemBlock::createOwning(const char *data, size_t size) {
    // Allocate block
    size_t totalSize = size + sizeof(MemBlock);
    void *block = ::operator new(totalSize);

    // Copy payload
    std::memcpy(static_cast<char *>(block) + sizeof(MemBlock), data, size);

    // Construct MemBlock
    MemBlock *memBlock =
        new (block) MemBlock(static_cast<const char *>(block), size, true);
    return std::move(*memBlock);
}

MemBlock MemBlock::createOwning(std::function<void(char *)> filler,
                                size_t size) {
    // Allocate block
    size_t totalSize = size + sizeof(MemBlock);
    void *block = ::operator new(totalSize);

    // Load payload
    filler(static_cast<char *>(block));

    // Construct MemBlock
    MemBlock *memBlock =
        new (block) MemBlock(static_cast<const char *>(block), size, true);
    return std::move(*memBlock);
}

MemBlock MemBlock::createNonOwning(const char *data, size_t size) {
    return MemBlock(data, size, false);
}

MemBlock::MemBlock(const char *data, size_t size, bool owning)
    : data_(data), size_(size), owning_(owning) {}

} // namespace cppelf::utils