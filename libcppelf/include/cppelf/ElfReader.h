
#ifndef TRAMPOLYN_CPPELF_ELFREADER_H
#define TRAMPOLYN_CPPELF_ELFREADER_H

#include "cppelf/ElfTypesFacade.h"
#include "cppelf/Enums.h"

#include <memory>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

namespace cppelf {

namespace detail {

class ElfReader {
public:
    virtual ~ElfReader() = default;

    virtual bool open() = 0;
    virtual bool close() = 0;

    virtual ElfHeader readHeader() const = 0;
    virtual const std::vector<ElfSectionHeader> &readSectionHeaders() const = 0;

    virtual std::vector<char> readRawData(size_t offset, size_t size) const = 0;
};

template <ElfClass EClass> class ElfClassFileReader : public ElfReader {
    static constexpr auto LayoutType = elfClassToLayout(EClass);
    using LayoutHeaderType = layout::ElfHeader<LayoutType>;
    using LayoutSectionHeaderType = layout::ElfSectionHeader<LayoutType>;

public:
    ElfClassFileReader(std::string_view path);

    bool open() override;
    bool close() override;
    ElfHeader readHeader() const override;
    const std::vector<ElfSectionHeader> &readSectionHeaders() const override;

    std::vector<char> readRawData(size_t offset, size_t size) const override;

private:
    void discoverLayout();

private:
    std::string_view path_;

    std::optional<ElfHeader> header_;
    std::vector<ElfSectionHeader> sectionHeaders_;
};

} // namespace detail

template <typename T>
concept IsConstructibleFromDataAndHeader =
    requires(const ElfSectionHeader &header, std::vector<char> &&data,
             const T &t) { T{header, std::move(data)}; };

class ElfFileReader {
public:
    ElfFileReader(std::string_view path);

public:
    ElfHeader header() const { return reader_->readHeader(); }
    const std::vector<ElfSectionHeader> &sectionHeaders() {
        return reader_->readSectionHeaders();
    }

    template <IsConstructibleFromDataAndHeader SectionType>
    std::shared_ptr<SectionType> retrieveSection(size_t idx) const {
        if (idx >= reader_->readSectionHeaders().size())
            return nullptr;

        ElfSectionHeader header = reader_->readSectionHeaders()[idx];
        return std::make_shared<SectionType>(
            header, reader_->readRawData(header.offset(), header.size()));
    }

private:
    std::unique_ptr<detail::ElfReader> reader_;
};

} // namespace cppelf

#endif
