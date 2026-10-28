#include "cppelf/layout/ElfTypes.h"
#include "cppelf/utils/MemView.h"

namespace cppelf::layout {

ElfGenericSectionView::ElfGenericSectionView(utils::MemView data)
    : data_(data) {}

ElfStringTableSectionView::ElfStringTableSectionView(utils::MemView data)
    : data_(data) {}

std::string_view ElfStringTableSectionView::lookup(size_t index) const {
    if (index == 0 || index >= data_.size())
        return {};

    const char *ptr = data_.data() + index;
    return std::string_view(ptr, std::strlen(ptr));
}

template <ElfClass EClass>
ElfSymbolTableSectionView<EClass>::ElfSymbolTableSectionView(
    utils::MemView data)
    : data_(data) {}

template <ElfClass EClass>
ElfSymbol<EClass>
ElfSymbolTableSectionView<EClass>::lookup(size_t index) const {
    return *reinterpret_cast<const ElfSymbol<EClass> *>(
        data_.data() + sizeof(ElfSymbol<EClass>) * index);
}

template class ElfSymbolTableSectionView<Elf32>;
template class ElfSymbolTableSectionView<Elf64>;

} // namespace cppelf::layout