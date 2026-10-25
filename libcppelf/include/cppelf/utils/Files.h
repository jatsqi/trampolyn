#ifndef TRAMPOLYN_CPPELF_UTILS_FILES_H
#define TRAMPOLYN_CPPELF_UTILS_FILES_H

#include "cppelf/utils/MemBlock.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace cppelf::utils {

/**
 * @brief Simple interface which represents a file.
 */
class File {
public:
    virtual ~File() = default;

    /**
     * @brief Read @ref size bytes starting from @ref offset.
     *
     * @param offset    Position to start reading from.
     * @param size      Number of bytes that should be read.
     * @return MemBlock Owning or non-owning representation of the read bytes.
     */
    virtual MemBlock read(size_t offset, size_t size) const = 0;
};

/**
 * @brief File which is mapped into memory.
 */
class MemoryMappedFile : public File {
public:
    MemoryMappedFile(std::string_view path);
    ~MemoryMappedFile();

    virtual MemBlock read(size_t offset, size_t size) const override;

protected:
    void loadRange(size_t offset, size_t size) const;

private:
    char *data_;
    uintmax_t size_;
};

/**
 * @brief Class which is aware that the underlying file might be read in
 *        "sections".
 */
class SectionedFile : public MemoryMappedFile {
public:
    struct Section {
        size_t offset, size;
    };

    struct SectionComp {
        bool operator()(const Section &s1, const Section &s2) const {
            return s1.offset < s2.offset;
        }
    };

public:
    SectionedFile(std::string_view path);
    SectionedFile(std::string_view path, const std::vector<Section> &sections);

public:
    MemBlock read(size_t offset, size_t len) const override;

    void addSection(Section section);

private:
    typename std::map<Section, MemBlock, SectionComp>::value_type *
    retrieveSection(Section section) const;
    typename std::map<Section, MemBlock, SectionComp>::value_type *
    findContainingSection(size_t offset, size_t size) const;

private:
    mutable std::map<Section, MemBlock, SectionComp> sections_;
};

} // namespace cppelf::utils

#endif