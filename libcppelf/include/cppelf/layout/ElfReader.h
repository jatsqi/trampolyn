#ifndef TRAMPOLYN_CPPELF_LAYOUT_ELFREADER_H
#define TRAMPOLYN_CPPELF_LAYOUT_ELFREADER_H

#include "cppelf/layout/ElfTypes.h"
#include "cppelf/utils/Files.h"

#include <cstddef>
#include <string_view>
#include <vector>

namespace cppelf::utils {
class MemBlock;
}

namespace cppelf::layout {

template <ElfClass EClass> class ElfFileReader {
public:
    ElfFileReader(std::string_view path);
    ~ElfFileReader() = default;
    ElfFileReader(ElfFileReader<EClass>&&) = default;
    ElfFileReader<EClass>& operator=(ElfFileReader<EClass>&&) = default;

public:
    ElfHeader<EClass> readHeader() const;
    std::vector<ElfSectionHeader<EClass>> readSectionHeaders() const;
    utils::MemBlock readSection(const ElfSectionHeader<EClass> &) const;
    utils::MemBlock readRaw(size_t offset, size_t size) const;

private:
    ElfHeader<EClass> retrieveHeader() const;
    std::vector<ElfSectionHeader<EClass>> retrieveSectionHeaders() const;

private:
    utils::SectionedFile file_;

    ElfHeader<EClass> cachedHeader_;
    std::vector<ElfSectionHeader<EClass>> cachedSections_;
};
using Elf32FileReader = ElfFileReader<Elf32>;
using Elf64FileReader = ElfFileReader<Elf64>;

} // namespace cppelf::layout

#endif