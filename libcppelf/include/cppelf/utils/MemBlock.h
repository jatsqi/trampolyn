#ifndef TRAMPOLYN_CPPELF_UTILS_MEMBLOCK_H
#define TRAMPOLYN_CPPELF_UTILS_MEMBLOCK_H

#include <cstddef>
#include <memory>

namespace cppelf::utils {

class MemBlock {
public:
    virtual ~MemBlock();

    size_t size() const { return size_; }
    bool isOwning() const { return owning_; }

    static std::unique_ptr<MemBlock> createOwning(const char *data, size_t size);
    static MemBlock createNonOwning(const char *data, size_t size);

private:
    MemBlock(const char *data, bool owning, size_t size);

private:
    size_t size_;
    bool owning_;
    const char *data_;
};

} // namespace cppelf::utils

#endif