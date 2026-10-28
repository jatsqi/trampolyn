#ifndef TRAMPOLYN_CPPELF_UTILS_MEMVIEW_H
#define TRAMPOLYN_CPPELF_UTILS_MEMVIEW_H

#include <cstddef>

namespace cppelf::utils {

class MemView {
public:
    MemView() = default;
    MemView(const char *data, size_t size) : data_(data), size_(size) {}

    const char* data() const { return data_; }
    size_t size() const { return size_; }

private:
    const char *data_;
    size_t size_;
};

} // namespace cppelf::utils

#endif