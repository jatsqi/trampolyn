#include "cppelf/ElfTypes.h"

namespace cppelf::layout {

ElfStringTableSection::ElfStringTableSection(std::vector<char> &&data)
    : data_(std::move(data)) {}

std::string_view ElfStringTableSection::lookup(size_t index) const {
    if (index == 0 || index >= data_.size())
        return {};

    const char *ptr = data_.data() + index;
    return std::string_view(ptr, std::strlen(ptr));
}

} // namespace cppelf::layout