#ifndef TRAMPOLYN_CPPELF_FLAGS_H
#define TRAMPOLYN_CPPELF_FLAGS_H

#include <bitset>
#include <cstddef>
#include <type_traits>
#include <limits>

namespace cppelf {

template <typename T>
concept EnumLosslesslyConvertibleToSizeT =
    std::is_enum_v<T> &&
    (std::numeric_limits<std::underlying_type_t<T>>::digits
        <= std::numeric_limits<std::size_t>::digits) &&
    (!std::is_signed_v<std::underlying_type_t<T>> ||
     std::numeric_limits<std::size_t>::digits >=
        std::numeric_limits<std::underlying_type_t<T>>::digits);

template<typename T>
requires EnumLosslesslyConvertibleToSizeT<T>
class Flags {
public:
    Flags(T init) : flags_(static_cast<size_t>(init)) {}

public:
    void toggle(T flag) {
        size_t f = static_cast<size_t>(flag);
        flags_.set(f);
    }

    bool check(T flag) const {
        return flags_.test(static_cast<size_t>(flag));
    }

private:
    std::bitset<sizeof(T) * 8> flags_;
};

} // namespace cppelf

#endif
