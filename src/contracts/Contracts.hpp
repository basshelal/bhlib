#pragma once

#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string_view>
#include <type_traits>
#include <utility>

#include "stacktrace/Stacktrace.hpp"

namespace contracts {
enum class ContractsLevel : std::uint8_t {
    NONE = 0,
    ALL = 1,
};

constexpr bool IS_CONTRACTS_ENABLED =
#ifdef PICAN_CONTRACTS_DISABLED
    false;
#else
    true;
#endif

constexpr ContractsLevel LEVEL = IS_CONTRACTS_ENABLED ? ContractsLevel::ALL : ContractsLevel::NONE;
using ViolationHandler = void (*)(std::string_view msg);

template<typename Callable>
class PostconditionChecker {
    Callable func_f;

public:
    constexpr explicit PostconditionChecker(Callable func) : func_f(std::move(func)) {
    }

    constexpr ~PostconditionChecker() noexcept(false) {
        if constexpr (LEVEL >= ContractsLevel::ALL) {
            this->func_f();
        }
    }

    PostconditionChecker(const PostconditionChecker&) = delete;
    PostconditionChecker(PostconditionChecker&&) = delete;
    PostconditionChecker&
    operator=(const PostconditionChecker&) = delete;
    PostconditionChecker&
    operator=(PostconditionChecker&&) = delete;
};

template<typename T>
class InvariantChecker {
    static_assert(
        requires(const T& t) {
            { t.invariants() } -> std::same_as<void>;
        }, "Class must define a public `void invariants() const` method to use class invariants"
    );

    const T* obj_f;

public:
    constexpr explicit InvariantChecker(const T* obj) : obj_f(obj) {
        if constexpr (LEVEL >= ContractsLevel::ALL) {
            this->obj_f->check_invariants();
        }
    }

    InvariantChecker(const InvariantChecker&) = delete;
    InvariantChecker(InvariantChecker&&) = delete;
    InvariantChecker&
    operator=(const InvariantChecker&) = delete;
    InvariantChecker&
    operator=(InvariantChecker&&) = delete;

    constexpr ~InvariantChecker() noexcept(false) {
        if constexpr (LEVEL >= ContractsLevel::ALL) {
            this->obj_f->check_invariants();
        }
    }
};

template<typename Callable>
class LoopInvariantChecker {
private:
    Callable func_f;

public:
    constexpr explicit LoopInvariantChecker(Callable func) : func_f(std::move(func)) {
        if constexpr (LEVEL >= ContractsLevel::ALL) {
            this->func_f();
        }
    }

    constexpr ~LoopInvariantChecker() noexcept {
        if constexpr (LEVEL >= ContractsLevel::ALL) {
            this->func_f();
        }
    }

    LoopInvariantChecker(const LoopInvariantChecker&) = delete;
    LoopInvariantChecker(LoopInvariantChecker&&) = delete;
    LoopInvariantChecker&
    operator=(const LoopInvariantChecker&) = delete;
    LoopInvariantChecker&
    operator=(LoopInvariantChecker&&) = delete;
};

inline void
_contract_violation_at_compile_time() {
    // No body because only called from a constexpr compile time so will always fail
}

[[noreturn]]
inline void
default_violation_handler(std::string_view msg) {
    stacktrace::print_stacktrace(stderr, 2);
    if (!msg.empty()) {
        std::fprintf(stderr, "Contract violation: %.*s\n", static_cast<int>(msg.size()), msg.data());
    } else {
        std::fprintf(stderr, "Contract violation!\n");
    }
    std::abort();
}

static_assert(std::is_same_v<decltype(&default_violation_handler), ViolationHandler>);

inline ViolationHandler current_violation_handler = default_violation_handler;

inline void
set_violation_handler(ViolationHandler handler) {
    current_violation_handler = handler;
}

constexpr void
report_violation(std::string_view msg) {
    if consteval {
        _contract_violation_at_compile_time();
    } else {
        if (current_violation_handler) {
            current_violation_handler(msg);
        } else {
            default_violation_handler(msg);
        }
    }
}

constexpr void
assertion(bool cond, std::string_view msg = "") {
    if constexpr (LEVEL >= ContractsLevel::ALL) {
        if (!cond) {
            report_violation(msg);
        }
    }
}

constexpr void
precondition(bool cond, std::string_view msg = "") {
    if constexpr (LEVEL >= ContractsLevel::ALL) {
        if (!cond) {
            report_violation(msg);
        }
    }
}

template<typename Callable>
[[nodiscard]]
constexpr PostconditionChecker<Callable>
postcondition(Callable func) {
    return PostconditionChecker<Callable>(std::move(func));
}

template<typename T>
[[nodiscard]]
constexpr InvariantChecker<T>
check_invariants(const T* obj) {
    return InvariantChecker<T>(obj);
}

template<typename Callable>
[[nodiscard]]
constexpr LoopInvariantChecker<Callable>
loop_invariant(Callable func) {
    return LoopInvariantChecker<Callable>(std::move(func));
}

}  // namespace contracts
