#ifndef TRAMPOLYN_CPPELF_LAYOUT_ELFTYPES_H
#define TRAMPOLYN_CPPELF_LAYOUT_ELFTYPES_H

#include "cppelf/layout/TypeDefs.h"

#include <cstdint>
#include <cstring>
#include <span>
#include <string_view>
#include <type_traits>

namespace cppelf::layout {

/**
 * Enum to characterize the different elf classes.
 *
 * We use enum instead of enum class on purpose.
 */
enum class ElfClass {
    Elf32 = 32,
    Elf64 = 64
};
constexpr ElfClass Elf32 = ElfClass::Elf32;
constexpr ElfClass Elf64 = ElfClass::Elf64;

/**
 * Simple helper to extract the ElfClass from a type.
 */
template<typename T>
struct ElfClassFromLayoutType;
template<template<ElfClass> typename T, ElfClass EClass>
struct ElfClassFromLayoutType<T<EClass>> {
    static constexpr ElfClass TypeElfClass = EClass;
};

template<ElfClass EClass>
struct ElfHeader;

template<ElfClass EClass>
struct ElfSymbol;

template<ElfClass EClass>
struct ElfRelocation;

template<ElfClass EClass>
struct ElfRelocationWithAddend;

template<ElfClass EClass>
struct ElfSectionHeader;

template<ElfClass EClass>
struct ElfSymbolTableSectionView;

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
template<>
struct ElfHeader<Elf32> {
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
using Elf32Header = ElfHeader<Elf32>;
static_assert(std::is_trivially_copyable_v<ElfHeader<Elf32>>);
static_assert(sizeof(ElfHeader<Elf32>) == 52);

/**
 * Layout the ELF 64 bit header.
 */
template<>
struct ElfHeader<Elf64> {
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
using Elf64Header = ElfHeader<Elf64>;
static_assert(std::is_trivially_copyable_v<ElfHeader<Elf64>>);
static_assert(sizeof(ElfHeader<Elf64>) == 64);

/**
 * Layout of a single ELF 32 symbol table entry.
 */
template<>
struct ElfSymbol<Elf32> {
    unsigned char type() const { return info & 0x0F; }
    unsigned char binding() const { return info >> 4; }

    Elf32_Word nameIdx;
    Elf32_Addr value;
    Elf32_Word size;
    unsigned char info;
    unsigned char other;
    Elf32_Half sectionHeaderTableIdx;
};
using Elf32Symbol = ElfSymbol<Elf32>;

/**
 * Layout of a single ELF 64 symbol table entry.
 */
template<>
struct ElfSymbol<Elf64> {
    unsigned char type() const { return info & 0x0F; }
    unsigned char binding() const { return info >> 4; }

    Elf64_Word nameIdx;
    unsigned char info;
    unsigned char other;
    Elf64_Half sectionHeaderTableIdx;
    Elf64_Addr value;
    Elf64_XWord size;
};
using Elf64Symbol = ElfSymbol<Elf64>;

/**
 * Layout of a ELF 32 relocation entry without addend.
 */
template<>
struct ElfRelocation<Elf32> {
    Elf32_Word symbolTableIndex() const { return info >> 8; }
    unsigned char type() const { return info & 0xFF; }

    Elf32_Addr offset;
    Elf32_Word info;
};
using Elf32Relocation = ElfRelocation<Elf32>;

/**
 * Layout of a ELF 32 relocation entry with addend.
 */
template<>
struct ElfRelocationWithAddend<Elf32> : public ElfRelocation<Elf32> {
    Elf32_Sword addend;
};
using Elf32RelocationWithAddend = ElfRelocationWithAddend<Elf32>;

/**
 * Layout of a ELF 64 relocation entry without addend.
 */
template<>
struct ElfRelocation<Elf64> {
    Elf64_XWord symbolTableIndex() const { return info >> 32; }
    Elf64_XWord type() const { return info & 0xFFFFFFFFull; }

    Elf64_Addr offset;
    Elf64_XWord info;
};
using Elf64Relocation = ElfRelocation<Elf64>;

/**
 * Layout of a ELF 64 relocation entry with addend.
 */
template<>
struct ElfRelocationWithAddend<Elf64> : public ElfRelocation<Elf64> {
    Elf64_SXWord addend;
};
using Elf64RelocationWithAddend = ElfRelocationWithAddend<Elf64>;

/**
 * Layout the ELF 32 bit section header.
 */
template<>
struct ElfSectionHeader<Elf32> {
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
using Elf32SectionHeader = ElfSectionHeader<Elf32>;
static_assert(std::is_trivially_copyable_v<ElfSectionHeader<Elf32>>);
static_assert(sizeof(ElfSectionHeader<Elf32>) == 40);

/**
 * Layout the ELF 64 bit section header.
 */
template<>
struct ElfSectionHeader<Elf64> {
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
using Elf64SectionHeader = ElfSectionHeader<Elf64>;
static_assert(std::is_trivially_copyable_v<ElfSectionHeader<Elf64>>);
static_assert(sizeof(ElfSectionHeader<Elf64>) == 64);

/**
 * ELF string table section.
 */
class ElfGenericSectionView {
public:
    ElfGenericSectionView(std::span<const char> data);

    std::span<const char> data() const { return data_; }
    size_t size() const { return data_.size(); }

private:
    std::span<const char> data_;
};

/**
 * ELF string table section.
 */
class ElfStringTableSectionView {
public:
    ElfStringTableSectionView(std::span<const char> data);

    std::span<const char> data() const { return data_; }
    size_t size() const { return data_.size(); }
    std::string_view lookup(size_t index) const;

private:
    std::span<const char> data_;
};

template<ElfClass EClass>
class ElfSymbolTableSectionView {
public:
    ElfSymbolTableSectionView(std::span<const char> data);

    std::span<const char> data() const { return data_; }
    size_t size() const { return data_.size(); }
    ElfSymbol<EClass> lookup(size_t index) const;

private:
    std::span<const char> data_;
};
using Elf32SymbolTableSectionView = ElfSymbolTableSectionView<Elf32>;
using Elf64SymbolTableSectionView = ElfSymbolTableSectionView<Elf64>;

} // namespace cppelf::layout

#endif
