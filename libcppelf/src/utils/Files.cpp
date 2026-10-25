#include "cppelf/utils/Files.h"
#include "cppelf/utils/MemBlock.h"

#include <cstdint>
#include <filesystem>
#include <fstream>

#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

namespace cppelf::utils {

MemoryMappedFile::MemoryMappedFile(std::string_view path) {
    size_ = std::filesystem::file_size(path);
    int fd = open(path.data(), O_RDONLY);

    void *dat = mmap(NULL, size_, PROT_READ, MAP_PRIVATE, fd, 0);
    data_ = static_cast<char *>(dat);
}

MemoryMappedFile::~MemoryMappedFile() {
    if (!data_)
        return;

    munlock(data_, size_);
    munmap(static_cast<void *>(data_), size_);
}

void MemoryMappedFile::loadRange(size_t offset, size_t size) const {
    mlock(data_ + offset, size);
}

SectionedFile::SectionedFile(std::string_view path) : MemoryMappedFile(path) {}

SectionedFile::SectionedFile(std::string_view path,
                             const std::vector<Section> &sections)
    : MemoryMappedFile(path) {
    for (const Section &sec : sections)
        sections_[sec] = nullptr;
}

MemBlock SectionedFile::read(size_t offset, size_t len) const {
    
}

typename std::map<SectionedFile::Section, std::unique_ptr<const MemBlock>,
                  SectionedFile::SectionComp>::value_type *
SectionedFile::retrieveSection(SectionedFile::Section section) const {}

typename std::map<SectionedFile::Section, std::unique_ptr<const MemBlock>,
                  SectionedFile::SectionComp>::value_type *
SectionedFile::findContainingSection(size_t offset, size_t size) const {
    auto it = sections_.upper_bound({.offset = offset, .size = 0});
    if (it == sections_.end())
        return nullptr;

    // Here, we have it->offset <= offset.
    const auto &[section, _] = *it;
    if (offset + size <= section.offset + section.size)
        return &*it;
    return nullptr;
}

} // namespace cppelf::utils