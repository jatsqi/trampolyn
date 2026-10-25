#ifndef TRAMPOLYN_CPPELF_LAYOUT_TYPEDEFS_H
#define TRAMPOLYN_CPPELF_LAYOUT_TYPEDEFS_H

#include <cstdint>

namespace cppelf {

using Elf32_Addr = uint32_t;
using Elf32_Half = uint16_t;
using Elf32_Off = uint32_t;
using Elf32_Sword = int32_t;
using Elf32_Word = uint32_t;

using Elf64_Addr = uint64_t;
using Elf64_Half = uint16_t;
using Elf64_Off = uint64_t;
using Elf64_Sword = int32_t;
using Elf64_Word = uint32_t;
using Elf64_XWord = uint64_t;
using Elf64_SXWord = uint64_t;

using Elf_Addr = Elf64_Addr;
using Elf_Half = Elf64_Half;
using Elf_Off = Elf64_Off;
using Elf_Sword = Elf64_Sword;
using Elf_Word = Elf64_Word;
using Elf_XWord = Elf64_XWord;
using Elf_SXWord = Elf64_SXWord;

} // namespace cppelf

#endif