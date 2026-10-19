#ifndef TRAMPOLYN_CPPELF_FACADE_H
#define TRAMPOLYN_CPPELF_FACADE_H

#include "cppelf/ElfTypes.h"
#include "cppelf/Flags.h"

#include <cstdint>
#include <type_traits>

namespace cppelf {

class ElfHeaderIdent {
public:
    ElfHeaderIdent(const layout::ElfHeaderIdent &ident)
        : magic_({.mag0_ = ident.magic_.mag0_,
                  .mag1_ = ident.magic_.mag1_,
                  .mag2_ = ident.magic_.mag2_,
                  .mag3_ = ident.magic_.mag3_}),
          class_(static_cast<ElfClass>(ident.class_)),
          dataEncoding_(static_cast<ElfEndianess>(ident.dataEncoding_)),
          version_(ident.version_), osABI_(ident.osABI_),
          osABIVersion_(ident.osABIVersion_) {}

public:
    ElfClass classs() const { return class_; }
    ElfEndianess dataEncoding() const { return dataEncoding_; }
    uint8_t version() const { return version_; }
    uint8_t osABI() const { return osABI_; }
    uint8_t osABIVersion() const { return osABIVersion_; }

private:
    struct {
        uint8_t mag0_;
        uint8_t mag1_;
        uint8_t mag2_;
        uint8_t mag3_;
    } magic_;
    ElfClass class_;
    ElfEndianess dataEncoding_;
    uint8_t version_;
    uint8_t osABI_;
    uint8_t osABIVersion_;
    uint8_t padding_[7];
};

class ElfHeader {
public:
    template <typename T>
        requires std::is_same_v<T, layout::Elf32Header> ||
                     std::is_same_v<T, layout::Elf64Header>
    ElfHeader(const T &header)
        : ident_(header.ident), type_(header.type), machine_(header.machine),
          version_(header.version), entry_(header.entry), phoff_(header.phoff),
          shoff_(header.shoff), flags_(header.flags), ehsize_(header.ehsize),
          phentsize_(header.phentsize), phnum_(header.phnum),
          shentsize_(header.shentsize), shnum_(header.shnum),
          shstrndx_(header.shstrndx) {}

public:
    const ElfHeaderIdent &ident() const { return ident_; }
    Elf_Half type() const { return type_; }
    Elf_Half machine() const { return machine_; }
    Elf_Word version() const { return version_; }
    Elf_Off programHeaderOffset() const { return phoff_; }
    Elf_Off sectionHeaderOffset() const { return shoff_; }
    Elf_Word flags() const { return flags_; }
    Elf_Half headerSize() const { return ehsize_; }
    Elf_Half programHeaderEntrySize() const { return phentsize_; }
    Elf_Half programHeaderNumEntries() const { return phnum_; }
    Elf_Half sectionHeaderSize() const { return shentsize_; }
    Elf_Half sectionHeaderNumEntries() const { return shnum_; }
    Elf_Half stringTableIdxInSectionHeader() const { return shstrndx_; }

private:
    ElfHeaderIdent ident_;
    Elf64_Half type_;
    Elf64_Half machine_;
    Elf64_Word version_;
    Elf64_Addr entry_;
    Elf64_Off phoff_;
    Elf64_Off shoff_;
    Elf64_Word flags_;
    Elf64_Half ehsize_;
    Elf64_Half phentsize_;
    Elf64_Half phnum_;
    Elf64_Half shentsize_;
    Elf64_Half shnum_;
    Elf64_Half shstrndx_;
};

class ElfSectionHeader {
public:
    Elf_Word nameIndex() const { return nameIdx_; }
    ElfSectionType type() const { return type_; }
    Elf_XWord flags() const { return flags_; }
    Elf_Addr address() const { return addr_; }
    Elf_Off offset() const { return offset_; }
    Elf_XWord size() const { return size_; }
    Elf_Word link() const { return link_; }
    Elf_Word info() const { return info_; }
    Elf_XWord addressAlign() const { return addressAlign_; }
    Elf_XWord entrySize() const { return entrySize_; }

private:
    Elf_Word nameIdx_;
    ElfSectionType type_;
    Elf_XWord flags_;
    Elf_Addr addr_;
    Elf_Off offset_;
    Elf_XWord size_;
    Elf_Word link_;
    Elf_Word info_;
    Elf_XWord addressAlign_;
    Elf_XWord entrySize_;
};

class ElfSymbol {
public:
    Elf_Word nameIndex() const { return nameIdx_; }
    const Flags<ElfSymbolType> &types() const { return types_; }
    const Flags<ElfSymbolBinding> &bindings() const { return bindings_; }

private:
    Elf_Word nameIdx_;
    Flags<ElfSymbolType> types_;
    Flags<ElfSymbolBinding> bindings_;
};

} // namespace cppelf

#endif
