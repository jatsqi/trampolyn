#include "cppelf/TypeDefs.h"

#include <cstdint>

namespace cppelf {

/**
 * Types of ELF files.
 */
enum class ElfClass : uint8_t {
    ELF32 = 1,
    ELF64 = 2
};

/**
 * The data is either stored in little endian or big endian.
 *
 * @note This library, however, currently only support little endian.
 */
enum class ElfEndianess : uint8_t {
    LITTLE = 1,
    BIG = 2
};

/**
 * List of all ELF section types.
 *
 * @note We use Efl32_Word here, regardless of the concrete class.
 */
enum class ElfSectionType : uint8_t {
    Null = 0,
    Progbits = 1,
    SymbolTable = 2,
    StringTable = 3,
    RelocationAddends = 4, // RELA
    Hash = 5,
    Dynamic = 6,
    Note = 7,
    NoBits = 8,
    RelocationWithoutAddends = 9, // REL
    ReservedButUnknown = 10,      // SHLIB
    DynamicSymbols = 11
};

/**
 * An ELF section can carry a number of flags.
 */
enum class ElfSectionFlag : Elf64_XWord {
    Write = 0x1, // Section should be writable during program execution.
    Alloc = 0x2, // Section occupies memory during program execution.
    ExecutableInstructions = 0x4, // Section contains executable instructions.
    MaskForProcessorReservedFlags = 0xf0000000
};

} // namespace cppelf