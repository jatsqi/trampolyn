#include "cppelf/utils/Files.h"
#include "cppelf/utils/MemBlock.h"

#include <filesystem>
#include <stdexcept>
#include <string>

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

namespace cppelf::utils {

MemoryMappedFile::MemoryMappedFile(std::string_view path)
    : size_(0), data_(nullptr) {
    size_ = std::filesystem::file_size(path);

    int fd = open(std::string(path).c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0)
        throw std::runtime_error("failed to open " + std::string(path) + ": " +
                                 std::strerror(errno));

    void *dat = mmap(NULL, size_, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    if (dat == MAP_FAILED)
        throw std::runtime_error("failed to mmap " + std::string(path) + ": " +
                                 std::strerror(errno));

    data_ = static_cast<char *>(dat);
}

MemoryMappedFile::~MemoryMappedFile() {
    if (!data_)
        return;

    munlock(data_, size_);
    munmap(static_cast<void *>(data_), size_);
}

MemoryMappedFile::MemoryMappedFile(MemoryMappedFile &&other) noexcept
    : data_(other.data_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = 0;
}

MemoryMappedFile &
MemoryMappedFile::operator=(MemoryMappedFile &&other) noexcept {
    if (&other == this)
        return *this;

    if (data_) {
        munlock(data_, size_);
        munmap(static_cast<void *>(data_), size_);
    }

    data_ = other.data_;
    size_ = other.size_;
    other.data_ = nullptr;
    other.size_ = 0;

    return *this;
}

MemBlock MemoryMappedFile::read(size_t offset, size_t size) const {
    if (offset + size > size_)
        return MemBlock{};
    return MemBlock::createNonOwning(data_ + offset, size);
}

void MemoryMappedFile::loadRange(size_t offset, size_t size) const {
    if (mlock(data_ + offset, size) != 0)
        throw std::runtime_error("mlock failed");
}

SectionedFile::SectionedFile(std::string_view path) : MemoryMappedFile(path) {}

SectionedFile::SectionedFile(std::string_view path,
                             const std::vector<Section> &sections)
    : MemoryMappedFile(path) {
    for (const Section &sec : sections)
        addSection(sec);
}

MemBlock SectionedFile::read(size_t offset, size_t len) const {
    const Section *section = findContainingSection(offset, len);
    if (!section)
        return MemoryMappedFile::read(offset, len);

    // Load whole section to avoid future reads
    MemoryMappedFile::loadRange(section->offset, section->size);
    return MemoryMappedFile::read(offset, len);
}

void SectionedFile::addSection(SectionedFile::Section section) {
    sections_.insert(section);
}

const SectionedFile::Section *
SectionedFile::findContainingSection(size_t offset, size_t size) const {
    // First section whose offset is strictly greater than `offset`; the
    // candidate containing `offset` is the one before it.
    auto it = sections_.upper_bound({.offset = offset, .size = 0});
    if (it == sections_.begin())
        return nullptr;

    --it;
    const Section &section = *it;

    // Guard against overflow in `offset + size`.
    if (size > section.size - (offset - section.offset))
        return nullptr;

    return &*it;
}

} // namespace cppelf::utils