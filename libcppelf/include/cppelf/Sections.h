#ifndef TRAMPOLYN_CPPELF_SECTIONS_H
#define TRAMPOLYN_CPPELF_SECTIONS_H

#include "cppelf/layout/ElfTypes.h"
#include "cppelf/ElfTypesFacade.h"

#include <cstddef>
#include <string_view>
#include <variant>
#include <vector>

namespace cppelf {

/**
 * Empty base class with virtual destructor.
 *
 * Acts as a "tag" so sections may be stored in one vector.
 */
class ElfSectionBase {
public:
    virtual ~ElfSectionBase() = default;
};

/**
 * ELF String Section.
 */
class ElfStringSection : public ElfSectionBase {
public:
    ElfStringSection(const ElfSectionHeader &header, std::vector<char> &&data);

    std::string_view getString(size_t index) const;
    size_t numBytes() const;

private:
    ElfSectionHeader header_;
    std::vector<char> data_;
    layout::ElfStringTableSectionView section_;
};

/**
 * ELF Symbol Table Section.
 */
class ElfSymbolTableSection : public ElfSectionBase {
public:
    ElfSymbolTableSection(const ElfSectionHeader &header,
                          std::vector<char> &&data);

    size_t numSymbols() const;
    ElfSymbol lookup(size_t index) const;

private:
    ElfSectionHeader header_;
    std::vector<char> data_;
    std::variant<layout::Elf32SymbolTableSectionView,
                 layout::Elf64SymbolTableSectionView>
        section_;
};

/**
 * ELF String Section.
 */
class ElfGenericSection : public ElfSectionBase {
public:
    ElfGenericSection(const ElfSectionHeader &header, std::vector<char> &&data);

    std::span<const char> data() const;
    size_t size() const;

private:
    ElfSectionHeader header_;
    std::vector<char> data_;
    layout::ElfGenericSectionView section_;
};

} // namespace cppelf

#endif