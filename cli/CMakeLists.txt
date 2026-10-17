list(APPEND TRAMPOLYN_CLI_SOURCES
    main.cpp
)
add_executable(trampolyn_cli
    ${TRAMPOLYN_CLI_SOURCES}
)
target_link_libraries(trampolyn_cli
    PRIVATE
        libtrampolyn_static
)
