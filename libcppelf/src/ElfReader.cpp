#include "cppelf/ElfReader.h"
#include "cppelf/ElfTypesFacade.h"
#include "cppelf/Enums.h"
#include "cppelf/layout/ElfReader.h"

#include <memory>
#include <vector>

namespace cppelf::facade {
namespace detail {

template <ElfClass EClass>
ElfClassFileReader<EClass>::ElfClassFileReader(std::string_view path)
    : file_(path), header_(file_.readHeader()) {
    for (const auto &sh : file_.readSectionHeaders())
        sectionHeaders_.emplace_back(sh);
}

template <ElfClass EClass> bool ElfClassFileReader<EClass>::open() {
    return true;
}

template <ElfClass EClass> bool ElfClassFileReader<EClass>::close() {
    return true;
}

template <ElfClass EClass>
ElfHeader ElfClassFileReader<EClass>::readHeader() const {
    return header_;
}

template <ElfClass EClass>
const std::vector<ElfSectionHeader> &
ElfClassFileReader<EClass>::readSectionHeaders() const {
    return sectionHeaders_;
}

template <ElfClass EClass>
utils::MemBlock ElfClassFileReader<EClass>::readRawData(size_t offset,
                                                        size_t size) const {
    return file_.readRaw(offset, size);
}

template class ElfClassFileReader<ElfClass::ELF32>;
template class ElfClassFileReader<ElfClass::ELF64>;

} // namespace detail

ElfFileReader::ElfFileReader(std::string_view path) {
    layout::ElfHeaderIdent ident = layout::readHeaderIdentFromPath(path);
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

} // namespace cppelf::facade