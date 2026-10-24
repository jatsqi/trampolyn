#include "cppelf/ElfReader.h"
#include "cppelf/ElfTypesFacade.h"
#include "cppelf/Enums.h"

#include <fstream>
#include <ios>
#include <memory>
#include <vector>

namespace cppelf {
namespace detail {

template <ElfClass EClass>
ElfClassFileReader<EClass>::ElfClassFileReader(std::string_view path)
    : path_(path) {
    discoverLayout();
}

template <ElfClass EClass> bool ElfClassFileReader<EClass>::open() {
    return true;
}

template <ElfClass EClass> bool ElfClassFileReader<EClass>::close() {
    return true;
}

template <ElfClass EClass>
ElfHeader ElfClassFileReader<EClass>::readHeader() const {
    return *header_;
}

template <ElfClass EClass>
const std::vector<ElfSectionHeader> &
ElfClassFileReader<EClass>::readSectionHeaders() const {
    return sectionHeaders_;
}

template <ElfClass EClass>
std::span<const char> ElfClassFileReader<EClass>::readBlob(size_t offset,
                                                           size_t size) const {
    std::ifstream stream(path_.data(),
                         std::ios_base::in | std::ios_base::binary);
    std::vector<char> storage(size);
    stream.read(storage.data(), size);
    blobs_.push_back(std::move(storage));

    return std::span<const char>(blobs_.back().data(), size);
}

template <ElfClass EClass> void ElfClassFileReader<EClass>::discoverLayout() {
    std::ifstream stream(path_.data(),
                         std::ios_base::in | std::ios_base::binary);

    // Reader header
    LayoutHeaderType header;
    stream.read(reinterpret_cast<char *>(&header), sizeof(header));
    header_ = ElfHeader(header);

    // Read section headers
    std::vector<LayoutSectionHeaderType> sections;
    sections.resize(header_->sectionHeaderNumEntries());
    stream.seekg(header_->sectionHeaderOffset());
    stream.read(reinterpret_cast<char *>(sections.data()),
                sizeof(LayoutSectionHeaderType) *
                    header_->sectionHeaderNumEntries());
    for (const LayoutSectionHeaderType &h : sections)
        sectionHeaders_.emplace_back(h);

    stream.close();
}

template class ElfClassFileReader<ElfClass::ELF32>;
template class ElfClassFileReader<ElfClass::ELF64>;

} // namespace detail

ElfFileReader::ElfFileReader(std::string_view path) {
    layout::ElfHeaderIdent ident;
    std::ifstream stream;
    stream.open(path.data(), std::ios_base::in);
    stream.read(reinterpret_cast<char *>(&ident), sizeof(ElfHeaderIdent));
    stream.close();

    ElfClass cls;
    cls = static_cast<ElfClass>(ident.class_);

    switch (cls) {
    case ElfClass::ELF32:
        reader_ =
            std::make_unique<detail::ElfClassFileReader<ElfClass::ELF32>>(path);
        break;
    case ElfClass::ELF64:
        reader_ =
            std::make_unique<detail::ElfClassFileReader<ElfClass::ELF64>>(path);
        break;
    }
}

} // namespace cppelf