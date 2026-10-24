#ifndef TRAMPOLYN_CPPELF_SECTIONS_H
#define TRAMPOLYN_CPPELF_SECTIONS_H

#include "cppelf/ElfTypes.h"
#include "cppelf/ElfTypesFacade.h"

#include <cstddef>
#include <span>
#include <string_view>

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
    ElfStringSection(const ElfSectionHeader& header, std::span<const char> data);

    std::string_view getString(size_t index) const;
    size_t numBytes() const;

private:
    ElfSectionHeader header_;
    layout::ElfStringTableSectionView section_;
};

/**
 * ELF String Section.
 */
class ElfGenericSection : public ElfSectionBase {
public:
    ElfGenericSection(const ElfSectionHeader& header, std::span<const char> data);

    std::span<const char> data() const;
    size_t size() const;

private:
    ElfSectionHeader header_;
    layout::ElfGenericSectionView section_;
};

}

#endif