#include "cppelf/ElfTypes.h"
#include "cppelf/Sections.h"

#include <cstring>
#include <string_view>
#include <variant>

namespace cppelf {

ElfStringSection::ElfStringSection(const ElfSectionHeader &header,
                                   std::vector<char> &&data)
    : header_(header), data_(std::move(data)),
      section_(std::span<const char>(data_.data(), data_.size())) {}

std::string_view ElfStringSection::getString(size_t index) const {
    if (index >= section_.size())
        return std::string_view{};

    const char *str = section_.data().data() + index;
    return std::string_view(str, std::strlen(str));
}

size_t ElfStringSection::numBytes() const {
    return section_.data().size_bytes();
}

ElfSymbolTableSection::ElfSymbolTableSection(const ElfSectionHeader &header,
                                             std::vector<char> &&data)
    : header_(header), data_(std::move(data)),
      section_(
          header.originalClass() == ElfClass::ELF32
              ? decltype(section_)(layout::Elf32SymbolTableSectionView(data_))
              : decltype(section_)(
                    layout::Elf64SymbolTableSectionView(data_))) {}

size_t ElfSymbolTableSection::numSymbols() const {
    return header_.size() / header_.entrySize();
}

ElfSymbol ElfSymbolTableSection::lookup(size_t index) const {
    return std::visit(
        [index](const auto &view) { return ElfSymbol(view.lookup(index)); },
        section_);
}

ElfGenericSection::ElfGenericSection(const ElfSectionHeader &header,
                                     std::vector<char> &&data)
    : header_(header), data_(std::move(data)),
      section_(std::span<const char>(data_.data(), data_.size())) {}

std::span<const char> ElfGenericSection::data() const {
    return section_.data();
}

size_t ElfGenericSection::size() const { return section_.size(); }

}; // namespace cppelf