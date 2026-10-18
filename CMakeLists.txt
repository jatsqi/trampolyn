cmake_minimum_required(VERSION 3.30)
project(trampolyn
    DESCRIPTION "Static and dynamic ELF binary patcher"
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

message(STATUS "==========================================")
message(STATUS " Configuring ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "==========================================")

# Detect OS
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(TRAMPOLYN_SYSTEM_NAME "Linux")
    set(TRAMPOLYN_IS_LINUX TRUE)
ELSEIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "Windows")
    set(TRAMPOLYN_SYSTEM_NAME "Windows")
    set(TRAMPOLYN_IS_WINDOWS TRUE)
ELSE()
    message(FATAL_ERROR "Unknown OS.")
ENDIF()
message(STATUS "Detected OS: ${TRAMPOLYN_SYSTEM_NAME}")

# Detect Compiler
IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(TRAMPOLYN_COMPILER "GCC")
    set(TRAMPOLYN_IS_GCC TRUE)
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(TRAMPOLYN_COMPILER "Clang")
    set(TRAMPOLYN_IS_CLANG TRUE)
ELSE()
    message(FATAL_ERROR "Unknown compiler.")
ENDIF()
message(STATUS "Detected Compiler: ${TRAMPOLYN_COMPILER}")

# Detect Architecture
IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
    set(TRAMPOLYN_SYSTEM_ARCH "X86")
    set(TRAMPOLYN_IS_X86 TRUE)
ELSEIF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)$")
    set(TRAMPOLYN_SYSTEM_ARCH "ARM64")
    set(TRAMPOLYN_IS_ARM TRUE)
ELSE()
    message(FATAL_ERROR "Unknown system architecture.")
endif()
message(STATUS "Detected architecture: ${TRAMPOLYN_SYSTEM_ARCH}")

add_subdirectory(libcppelf)
add_subdirectory(libtrampolyn)
add_subdirectory(cli)
