#include "cppelf/Enums.h"
#include "cppelf/TypeDefs.h"

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <vector>

namespace cppelf::layout {

/**
 * The ELF header ident is stored at the beginning of the ELF header.
 *
 * @note The structure of the ident part is identical for 32 bits and 64 bits.
 */
class ElfHeaderIdent {
public:
    ElfClass classs() const { return static_cast<ElfClass>(class_); }
    ElfEndianess dataEncoding() const {
        return static_cast<ElfEndianess>(dataEncoding_);
    }
    uint8_t version() const { return version_; }
    uint8_t osABI() const { return osABI_; }
    uint8_t osABIVersion() const { return osABIVersion_; }

    bool validateMagic() const {
        return magic_.mag0_ == 0x7f && magic_.mag1_ == 'E' &&
               magic_.mag2_ == 'L' && magic_.mag3_ == 'F';
    }

private:
    struct {
        uint8_t mag0_;
        uint8_t mag1_;
        uint8_t mag2_;
        uint8_t mag3_;
    } magic_;
    uint8_t class_;
    uint8_t dataEncoding_;
    uint8_t version_;
    uint8_t osABI_;
    uint8_t osABIVersion_;
    uint8_t padding_[7];
};
static_assert(sizeof(ElfHeaderIdent) == 16);
static_assert(std::is_trivially_copyable_v<ElfHeaderIdent>);

/**
 * Layout the ELF 32 bit header.
 */
class Elf32Header {
public:
    const ElfHeaderIdent &ident() const { return ident_; }
    Elf32_Half type() const { return type_; }
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
    Elf32_Half type_;
    Elf32_Half machine_;
    Elf32_Word version_;
    Elf32_Addr entry_;
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
static_assert(std::is_trivially_copyable_v<Elf32Header>);
static_assert(sizeof(Elf32Header) == 52);

/**
 * Layout the ELF 64 bit header.
 */
class Elf64Header {
public:
    const ElfHeaderIdent &ident() const { return ident_; }
    Elf64_Half type() const { return type_; }
    Elf64_Half machine() const { return machine_; }
    Elf64_Word version() const { return version_; }
    Elf64_Off programHeaderOffset() const { return phoff_; }
    Elf64_Off sectionHeaderOffset() const { return shoff_; }
    Elf64_Word flags() const { return flags_; }
    Elf64_Half headerSize() const { return ehsize_; }
    Elf64_Half programHeaderEntrySize() const { return phentsize_; }
    Elf64_Half programHeaderNumEntries() const { return phnum_; }
    Elf64_Half sectionHeaderSize() const { return shentsize_; }
    Elf64_Half sectionHeaderNumEntries() const { return shnum_; }
    Elf64_Half stringTableIdxInSectionHeader() const { return ehsize_; }

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
static_assert(std::is_trivially_copyable_v<Elf64Header>);
static_assert(sizeof(Elf64Header) == 64);

/**
 * Layout the ELF 32 bit section header.
 */
class Elf32SectionHeader {
public:
    static constexpr Elf32_Word ProcessorTypeRange[2] = {0x70000000,
                                                         0x7fffffff};
    static constexpr Elf32_Word ApplicationTypeRange[2] = {0x80000000,
                                                           0xffffffff};

public:
    Elf32_Word nameIndex() const { return nameIdx_; }
    ElfSectionType type() const { return static_cast<ElfSectionType>(type_); }
    Elf32_Word flags() const { return flags_; }
    Elf32_Addr address() const { return addr_; }
    Elf32_Off offset() const { return offset_; }
    Elf32_Word size() const { return size_; }
    Elf32_Word link() const { return link_; }
    Elf32_Word info() const { return info_; }
    Elf32_Word addressAlign() const { return addressAlign_; }
    Elf32_Word entrySize() const { return entrySize_; }

    bool isProcessorReservedSection() const {
        return type_ >= ProcessorTypeRange[0] && type_ <= ProcessorTypeRange[1];
    }
    bool isApplicationReservedSection() const {
        return type_ >= ApplicationTypeRange[0] &&
               type_ <= ApplicationTypeRange[1];
    }

private:
    Elf32_Word nameIdx_;
    Elf32_Word type_;
    Elf32_Word flags_;
    Elf32_Addr addr_;
    Elf32_Off offset_;
    Elf32_Word size_;
    Elf32_Word link_;
    Elf32_Word info_;
    Elf32_Word addressAlign_;
    Elf32_Word entrySize_;
};
static_assert(std::is_trivially_copyable_v<Elf32SectionHeader>);
static_assert(sizeof(Elf32SectionHeader) == 40);

/**
 * Layout the ELF 64 bit section header.
 */
class Elf64SectionHeader {
public:
    static constexpr Elf32_Word ApplicationTypeRange[2] = {0x60000000,
                                                           0x6FFFFFFF};
    static constexpr Elf32_Word ProcessorTypeRange[2] = {0x70000000,
                                                         0x7fffffff};

public:
    Elf64_Word nameIndex() const { return nameIdx_; }
    ElfSectionType type() const { return static_cast<ElfSectionType>(type_); }
    Elf64_XWord flags() const { return flags_; }
    Elf64_Addr address() const { return addr_; }
    Elf64_Off offset() const { return offset_; }
    Elf64_XWord size() const { return size_; }
    Elf64_Word link() const { return link_; }
    Elf64_Word info() const { return info_; }
    Elf64_XWord addressAlign() const { return addressAlign_; }
    Elf64_XWord entrySize() const { return entrySize_; }

    bool isProcessorReservedSection() const {
        return type_ >= ProcessorTypeRange[0] && type_ <= ProcessorTypeRange[1];
    }
    bool isApplicationReservedSection() const {
        return type_ >= ApplicationTypeRange[0] &&
               type_ <= ApplicationTypeRange[1];
    }

private:
    Elf64_Word nameIdx_;
    Elf64_Word type_;
    Elf64_XWord flags_;
    Elf64_Addr addr_;
    Elf64_Off offset_;
    Elf64_XWord size_;
    Elf64_Word link_;
    Elf64_Word info_;
    Elf64_XWord addressAlign_;
    Elf64_XWord entrySize_;
};
static_assert(std::is_trivially_copyable_v<Elf64SectionHeader>);
static_assert(sizeof(Elf64SectionHeader) == 64);

/**
 * ELF string table section.
 */
class ElfStringTableSection {
public:
    ElfStringTableSection(std::vector<char> &&data);

    std::string_view lookup(size_t index) const;

private:
    std::vector<char> data_;
};

} // namespace cppelf::layout