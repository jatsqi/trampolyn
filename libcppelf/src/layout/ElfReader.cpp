#include "cppelf/layout/ElfReader.h"
#include "cppelf/layout/ElfTypes.h"
#include "cppelf/utils/MemBlock.h"

#include <array>
#include <bit>
#include <cstring>
#include <fstream>
#include <ios>

namespace cppelf::layout {

ElfHeaderIdent readHeaderIdentFromPath(std::string_view path) {
    std::ifstream in(std::string(path.data()).c_str());
    in.seekg(0, std::ios_base::beg);

    std::array<char, sizeof(ElfHeaderIdent)> data;
    in.read(data.data(), data.size());
    return std::bit_cast<ElfHeaderIdent>(data);
}

template <ElfClass EClass>
ElfFileReader<EClass>::ElfFileReader(std::string_view path)
    : file_(path), cachedHeader_(retrieveHeader()),
      cachedSections_(retrieveSectionHeaders()) {}

template <ElfClass EClass>
ElfHeader<EClass> ElfFileReader<EClass>::readHeader() const {
    return cachedHeader_;
}

template <ElfClass EClass>
std::vector<ElfSectionHeader<EClass>>
ElfFileReader<EClass>::readSectionHeaders() const {
    return cachedSections_;
}

template <ElfClass EClass>
utils::MemBlock ElfFileReader<EClass>::readSection(
    const ElfSectionHeader<EClass> &header) const {
    return file_.read(header.offset, header.size);
}

template <ElfClass EClass>
utils::MemBlock ElfFileReader<EClass>::readRaw(size_t offset, size_t size) const {
    return file_.read(offset, size);
}

template <ElfClass EClass>
ElfHeader<EClass> ElfFileReader<EClass>::retrieveHeader() const {
    ElfHeader<EClass> header;
    utils::MemBlock data = file_.read(0ul, sizeof(ElfHeader<EClass>));
    memcpy(reinterpret_cast<char *>(&header), data.data(),
           sizeof(ElfHeader<EClass>));
    return header;
}

template <ElfClass EClass>
std::vector<ElfSectionHeader<EClass>>
ElfFileReader<EClass>::retrieveSectionHeaders() const {
    std::vector<ElfSectionHeader<EClass>> sections;
    sections.resize(cachedHeader_.shnum);

    size_t numBytes = cachedHeader_.shnum * sizeof(ElfSectionHeader<EClass>);
    utils::MemBlock block = file_.read(cachedHeader_.shoff, numBytes);
    memcpy(reinterpret_cast<char *>(sections.data()), block.data(), numBytes);
    return sections;
}

template class ElfFileReader<Elf32>;
template class ElfFileReader<Elf64>;

} // namespace cppelf::layout