# Source files for ELF sublib
list(APPEND TRAMPOLYN_ELF_SOURCE_FILES
    ElfReader.cpp
    ElfTypes.cpp
    Section.cpp
)

# Add objlib for linking with other targets
add_library(libtrampolyn_elf_objlib
    OBJECT
        ${TRAMPOLYN_ELF_SOURCE_FILES})

# Bookkeeping of properties
target_include_directories(libtrampolyn_elf_objlib
    PUBLIC
        "${PROJECT_SOURCE_DIR}/libcppelf/include"
)
set_target_properties(libtrampolyn_elf_objlib PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
