#ifndef TRAMPOLYN_CPPELF_UTILS_MEMBLOCK_H
#define TRAMPOLYN_CPPELF_UTILS_MEMBLOCK_H

#include <cstdint>
#include <functional>

namespace cppelf::utils {

class MemBlock {
public:
    MemBlock();
    virtual ~MemBlock();

    MemBlock(const MemBlock &) = delete;
    MemBlock &operator=(const MemBlock &) = delete;
    MemBlock(MemBlock &&) noexcept;
    MemBlock &operator=(MemBlock &&) noexcept;

    size_t size() const { return size_; }
    bool isOwning() const { return owning_; }

    const char *data() const {
        if (!owning_)
            return data_;
        return *reinterpret_cast<const char *const *>(
            reinterpret_cast<const char *>(this) + sizeof(MemBlock));
    }

    static MemBlock createOwning(const char *data, size_t size);
    static MemBlock createOwning(std::function<void(char *)> filler,
                                 size_t size);
    static MemBlock createNonOwning(const char *data, size_t size);

private:
    MemBlock(const char* data, size_t size, bool owning);

private:
    const char* data_;
    size_t size_;
    bool owning_;
};

} // namespace cppelf::utils

#endif
