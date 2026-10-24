
#ifndef TRAMPOLYN_CPPELF_ELFREADER_H
#define TRAMPOLYN_CPPELF_ELFREADER_H

#include "cppelf/ElfTypesFacade.h"
#include "cppelf/Enums.h"

#include <memory>
#include <optional>
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
};

template <ElfClass EClass> class ElfClassFileReader : public ElfReader {
    using LayoutHeaderType = ElfClassToTypes<EClass>::HeaderType;
    using LayoutSectionHeaderType = ElfClassToTypes<EClass>::SectionHeaderType;

public:
    ElfClassFileReader(std::string_view path);

    bool open() override;
    bool close() override;
    ElfHeader readHeader() const override;
    const std::vector<ElfSectionHeader> &readSectionHeaders() const override;

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
    requires(const ElfSectionHeader &header, std::span<const char> data,
             const T &t) { T{header, data}; };

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
        if (idx >= sectionHeaders_.size())
            return nullptr;

        auto ptr = std::make_shared<SectionType>(sectionHeaders_[idx]);
        return ptr;
    }

private:
    std::unique_ptr<detail::ElfReader> reader_;
};

} // namespace cppelf

#endif
