#include "cppelf/ElfTypes.h"

namespace cppelf::layout {

ElfGenericSectionView::ElfGenericSectionView(std::span<const char> data)
    : data_(data) {}

ElfStringTableSectionView::ElfStringTableSectionView(std::span<const char> data)
    : data_(data) {}

std::string_view ElfStringTableSectionView::lookup(size_t index) const {
    if (index == 0 || index >= data_.size())
        return {};

    const char *ptr = data_.data() + index;
    return std::string_view(ptr, std::strlen(ptr));
}

} // namespace cppelf::layout