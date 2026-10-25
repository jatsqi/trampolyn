#ifndef TRAMPOLYN_CPPELF_LAYOUT_ELFREADER_H
#define TRAMPOLYN_CPPELF_LAYOUT_ELFREADER_H

#include "cppelf/layout/ElfTypes.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cppelf::layout {

template<ElfClass EClass>
class ElfFileReader {
public:
    ElfFileReader(std::string_view path);

public:
    ElfHeader<EClass> readHeader() const;
    std::vector<ElfSectionHeader<EClass>> readSectionHeaders() const;

private:
    std::string path_;

    std::optional<ElfHeader<EClass>> cachedHeader_;
    std::optional<std::vector<ElfSectionHeader<EClass>>> cachedSections_;
};
using Elf32FileReader = ElfFileReader<Elf32>;
using Elf64FileReader = ElfFileReader<Elf64>;

}

#endif