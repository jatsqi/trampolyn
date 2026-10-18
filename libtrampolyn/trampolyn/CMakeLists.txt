
list(APPEND TRAMPOLYN_SOURCE_FILES
    xyz.cpp
)
add_library(libtrampolyn_objlib
    OBJECT
        ${TRAMPOLYN_SOURCE_FILES})
target_include_directories(libtrampolyn_objlib
    PUBLIC
        "${PROJECT_SOURCE_DIR}/libtrampolyn/include"
)
set_target_properties(libtrampolyn_objlib PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Libraries
add_library(libtrampolyn SHARED)
target_link_libraries(libtrampolyn
    PUBLIC
        libtrampolyn_objlib
        libtrampolyn_elf_objlib
)

add_library(libtrampolyn_static)
target_link_libraries(libtrampolyn_static
    PUBLIC
        libtrampolyn_objlib
        libtrampolyn_elf_objlib
)