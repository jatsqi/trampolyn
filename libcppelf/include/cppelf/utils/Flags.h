#ifndef TRAMPOLYN_CPPELF_UTILS_FLAGS_H
#define TRAMPOLYN_CPPELF_UTILS_FLAGS_H

#include <bitset>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace cppelf::utils {

// Technically not required, just for testing.
template <typename T>
concept EnumLosslesslyConvertibleToSizeT =
    std::is_enum_v<T> &&
    (std::numeric_limits<std::underlying_type_t<T>>::digits <=
     std::numeric_limits<std::size_t>::digits) &&
    (!std::is_signed_v<std::underlying_type_t<T>> ||
     std::numeric_limits<std::size_t>::digits >=
         std::numeric_limits<std::underlying_type_t<T>>::digits);

/**
 * Class to store a set of flags.
 *
 * Often Enums are used to store bitmasks, e.g.:
 *
 * enum class MyFlags {
 *     FLAG_1 = 0x01,
 *     FLAG_2 = 0x02,
 *     ...
 * };
 *
 * Thus, we need some convenient way of storing a set of those flags.
 */
template <typename T>
    requires EnumLosslesslyConvertibleToSizeT<T>
class Flags {
public:
    using Underlying = std::underlying_type<T>;
    static constexpr size_t RequiredBits = sizeof(Underlying) * 8;

    Flags(T init) : flags_(static_cast<size_t>(init)) {}
    Flags(size_t init) : flags_(init) {}

public:
    void toggle(T flag) {
        size_t f = static_cast<size_t>(flag);
        // Detect "zero" bit mask.
        if (f == 0)
            return;
        flags_.set(f - 1);
    }

    bool check(T flag) const {
        size_t f = static_cast<size_t>(flag);
        // Detect "zero" bit mask.
        if (f == 0)
            return flags_.none();
        return flags_.test(f - 1);
    }

private:
    std::bitset<RequiredBits> flags_;
};

} // namespace cppelf

#endif
