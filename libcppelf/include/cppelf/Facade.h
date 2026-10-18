#include "cppelf/ElfTypes.h"

#include <cstdint>

namespace cppelf {

class ElfHeaderIdent : private layout::ElfHeaderIdent {
public:
    using layout::ElfHeaderIdent::classs;
    using layout::ElfHeaderIdent::dataEncoding;
    using layout::ElfHeaderIdent::version;
    using layout::ElfHeaderIdent::osABI;
    using layout::ElfHeaderIdent::osABIVersion;
    using layout::ElfHeaderIdent::validateMagic;
};

class ElfHeader {
public:
    ElfHeader(const layout::Elf32Header& elfHeader) : internalHeader_(elfHeader) {}
    ElfHeader(const layout::Elf64Header& elfHeader) : internalHeader_(elfHeader) {}

public:
    const ElfHeaderIdent &ident() const { return ident_; }
    uint32_t type() const { return type_; }
    Elf32_Half machine() const { return machine_; }
    Elf32_Word version() const { return version_; }
    Elf32_Off programHeaderOffset() const { return phoff_; }
    Elf32_Off sectionHeaderOffset() const { return shoff_; }
    Elf32_Word flags() const { return flags_; }
    Elf32_Half headerSize() const { return ehsize_; }
    Elf32_Half programHeaderEntrySize() const { return phentsize_; }
    Elf32_Half programHeaderNumEntries() const { return phnum_; }
    Elf32_Half sectionHeaderSize() const { return shentsize_; }
    Elf32_Half sectionHeaderNumEntries() const { return shnum_; }
    Elf32_Half stringTableIdxInSectionHeader() const { return ehsize_; }

private:
    ElfHeaderIdent ident_;
    uint16_t type_;
    uint16_t machine_;
    uint32_t version_;
    uint64_t entry_;
    Elf32_Off phoff_;
    Elf32_Off shoff_;
    Elf32_Word flags_;
    Elf32_Half ehsize_;
    Elf32_Half phentsize_;
    Elf32_Half phnum_;
    Elf32_Half shentsize_;
    Elf32_Half shnum_;
    Elf32_Half shstrndx_;
};

} // namespace cppelf