#pragma once

#include <utility>

#include "bhlib/core/contracts.hpp"

namespace bhlib {
template<typename TP, typename Tag_TP>
class TypeDef {
private:
    TP value_f;

public:  // types
    using ValueType = TP;

public:  // constructors
    TypeDef() = default;

    explicit constexpr TypeDef(const TP& value) : value_f{value} {
        BHLIB_PRECONDITION(sizeof(TypeDef) == sizeof(TP));
        BHLIB_PRECONDITION(
            reinterpret_cast<void*>(std::addressof(*this)) == reinterpret_cast<void*>(std::addressof(this->value_f))
        );
    }

    explicit constexpr TypeDef(TP&& value) : value_f{std::move(value)} {
        BHLIB_PRECONDITION(sizeof(TypeDef) == sizeof(TP));
        BHLIB_PRECONDITION(
            reinterpret_cast<void*>(std::addressof(*this)) == reinterpret_cast<void*>(std::addressof(this->value_f))
        );
    }

public:  // copy-control
    TypeDef(const TypeDef& rhs) = default;

    TypeDef(TypeDef&& rhs) = default;

    TypeDef&
    operator=(const TypeDef& rhs) = default;

    TypeDef&
    operator=(TypeDef&& rhs) = default;

    ~TypeDef() = default;

public:  // getters
    [[nodiscard]]
    const TP&
    get() const {
        return this->value_f;
    }

    [[nodiscard]]
    TP&
    get() {
        return this->value_f;
    }

    /**
     * Force all conversions to be explicit
     */
    [[nodiscard]]
    explicit
    operator TP() const {
        return this->value_f;
    }

    [[nodiscard]]
    bool
    operator<=>(const TypeDef&) const = default;
};
}  // namespace bhlib
