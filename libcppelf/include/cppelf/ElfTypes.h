#ifndef TRAMPOLYN_CPPELF_ELFTYPES_H
#define TRAMPOLYN_CPPELF_ELFTYPES_H

#include "cppelf/Enums.h"
#include "cppelf/TypeDefs.h"

#include <cstdint>
#include <cstring>
#include <string_view>
#include <type_traits>
#include <vector>

namespace cppelf::layout {

/**
 * The ELF header ident is stored at the beginning of the ELF header.
 *
 * @note The structure of the ident part is identical for 32 bits and 64 bits.
 */
struct ElfHeaderIdent {
    bool validateMagic() const {
        return magic_.mag0_ == 0x7f && magic_.mag1_ == 'E' &&
               magic_.mag2_ == 'L' && magic_.mag3_ == 'F';
    }

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
    ElfHeaderIdent ident;
    Elf32_Half type;
    Elf32_Half machine;
    Elf32_Word version;
    Elf32_Addr entry;
    Elf32_Off phoff;
    Elf32_Off shoff;
    Elf32_Word flags;
    Elf32_Half ehsize;
    Elf32_Half phentsize;
    Elf32_Half phnum;
    Elf32_Half shentsize;
    Elf32_Half shnum;
    Elf32_Half shstrndx;
};
static_assert(std::is_trivially_copyable_v<Elf32Header>);
static_assert(sizeof(Elf32Header) == 52);

/**
 * Layout the ELF 64 bit header.
 */
struct Elf64Header {
    ElfHeaderIdent ident;
    Elf64_Half type;
    Elf64_Half machine;
    Elf64_Word version;
    Elf64_Addr entry;
    Elf64_Off phoff;
    Elf64_Off shoff;
    Elf64_Word flags;
    Elf64_Half ehsize;
    Elf64_Half phentsize;
    Elf64_Half phnum;
    Elf64_Half shentsize;
    Elf64_Half shnum;
    Elf64_Half shstrndx;
};
static_assert(std::is_trivially_copyable_v<Elf64Header>);
static_assert(sizeof(Elf64Header) == 64);

/**
 * Layout of a single ELF 32 symbol table entry.
 */
struct Elf32Symbol {
    unsigned char type() const { return info & 0x0F; }
    unsigned char binding() const { return info >> 4; }

    Elf32_Word nameIdx;
    Elf32_Addr value;
    Elf32_Word size;
    unsigned char info;
    unsigned char other;
    Elf32_Half sectionHeaderTableIdx;
};

/**
 * Layout of a single ELF 64 symbol table entry.
 */
struct Elf64Symbol {
    unsigned char type() const { return info & 0x0F; }
    unsigned char binding() const { return info >> 4; }

    Elf64_Word nameIdx;
    unsigned char info;
    unsigned char other;
    Elf64_Half sectionHeaderTableIdx;
    Elf64_Addr value;
    Elf64_XWord size;
};

/**
 * Layout of a ELF 32 relocation entry without addend.
 */
struct Elf32Relocation {
    Elf32_Word symbolTableIndex() const { return info >> 8; }
    unsigned char type() const { return info & 0xFF; }

    Elf32_Addr offset;
    Elf32_Word info;
};

/**
 * Layout of a ELF 32 relocation entry with addend.
 */
struct Elf32RelocationWithAddend : public Elf32Relocation {
    Elf32_Sword addend;
};

/**
 * Layout of a ELF 64 relocation entry without addend.
 */
struct Elf64Relocation {
    Elf64_XWord symbolTableIndex() const { return info >> 32; }
    Elf64_XWord type() const { return info & 0xFFFFFFFFull; }

    Elf64_Addr offset;
    Elf64_XWord info;
};

/**
 * Layout of a ELF 64 relocation entry with addend.
 */
struct Elf64RelocationWithAddend : public Elf64Relocation {
    Elf64_SXWord addend;
};

/**
 * Layout the ELF 32 bit section header.
 */
struct Elf32SectionHeader {
    static constexpr Elf32_Word ProcessorTypeRange[2] = {0x70000000,
                                                         0x7fffffff};
    static constexpr Elf32_Word ApplicationTypeRange[2] = {0x80000000,
                                                           0xffffffff};

    bool isProcessorReservedSection() const {
        return type >= ProcessorTypeRange[0] && type <= ProcessorTypeRange[1];
    }
    bool isApplicationReservedSection() const {
        return type >= ApplicationTypeRange[0] &&
               type <= ApplicationTypeRange[1];
    }

    Elf32_Word nameIdx;
    Elf32_Word type;
    Elf32_Word flags;
    Elf32_Addr addr;
    Elf32_Off offset;
    Elf32_Word size;
    Elf32_Word link;
    Elf32_Word info;
    Elf32_Word addressAlign;
    Elf32_Word entrySize;
};
static_assert(std::is_trivially_copyable_v<Elf32SectionHeader>);
static_assert(sizeof(Elf32SectionHeader) == 40);

/**
 * Layout the ELF 64 bit section header.
 */
struct Elf64SectionHeader {
    static constexpr Elf32_Word ApplicationTypeRange[2] = {0x60000000,
                                                           0x6FFFFFFF};
    static constexpr Elf32_Word ProcessorTypeRange[2] = {0x70000000,
                                                         0x7fffffff};

    bool isProcessorReservedSection() const {
        return type >= ProcessorTypeRange[0] && type <= ProcessorTypeRange[1];
    }
    bool isApplicationReservedSection() const {
        return type >= ApplicationTypeRange[0] &&
               type <= ApplicationTypeRange[1];
    }

    Elf64_Word nameIdx;
    Elf64_Word type;
    Elf64_XWord flags;
    Elf64_Addr addr;
    Elf64_Off offset;
    Elf64_XWord size;
    Elf64_Word link;
    Elf64_Word info;
    Elf64_XWord addressAlign;
    Elf64_XWord entrySize;
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

#endif
