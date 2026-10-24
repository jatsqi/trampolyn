#include "cppelf/Sections.h"

#include <cstring>
#include <string_view>

namespace cppelf {

ElfStringSection::ElfStringSection(const ElfSectionHeader &header,
                                   std::span<const char> data)
    : header_(header), section_(data) {}

std::string_view ElfStringSection::getString(size_t index) const {
    if (index >= section_.size())
        return std::string_view{};

    const char *str = section_.data().data() + index;
    return std::string_view(str, std::strlen(str));
}

size_t ElfStringSection::numBytes() const { return section_.data().size_bytes(); }

ElfGenericSection::ElfGenericSection(const ElfSectionHeader &header,
                                   std::span<const char> data)
    : header_(header), section_(data) {}

std::span<const char> ElfGenericSection::data() const {
    return section_.data();
}

size_t ElfGenericSection::size() const {
    return section_.size();
}


}; // namespace cppelf