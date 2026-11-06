#include <random>
#include <type_traits>

#include <catch2/catch_all.hpp>

#include "bhlib/core/core.hpp"

namespace {
template<typename T>
constexpr bool
is_power_of_two(T x) noexcept {
    static_assert(std::is_unsigned_v<T>, "Only unsigned types supported");
    return x != 0 && (x & (x - 1)) == 0;
}

template<std::size_t SizeBytes_V>
struct Sized {
    static_assert(is_power_of_two(SizeBytes_V));
    std::array<std::byte, SizeBytes_V> data;

    [[nodiscard]]
    static Sized
    generate_random() {
        // TODO(bxh) 06-Nov-26 23:56 Create test helper for random generators!
        std::mt19937 generator(std::random_device{}.operator()());
        std::uniform_int_distribution<uint16_t> dist(0, 255);

        Sized result;
        for (std::size_t i = 0; i < SizeBytes_V; ++i) {
            result.data[i] = static_cast<std::byte>(dist(generator));
        }

        return result;
    }
};

template<typename TypeDef_TP, typename Value_TP>
struct Tester {
    Tester() {
        STATIC_REQUIRE(sizeof(TypeDef_TP) == sizeof(Value_TP));
        STATIC_REQUIRE(std::is_const_v<TypeDef_TP> == std::is_const_v<Value_TP>);
    }

    template<typename TypeDef>
    void
    test(TypeDef& def) {
        STATIC_REQUIRE(std::is_same_v<TypeDef_TP, TypeDef> || std::is_same_v<TypeDef_TP, const TypeDef>);

        Value_TP& gotValue = def.get();
        const auto* defAddress = reinterpret_cast<const std::byte*>(std::addressof(def));
        const auto* valueAddress = reinterpret_cast<const std::byte*>(std::addressof(gotValue));
        CHECK(defAddress == valueAddress);
    }
};
}  // namespace

namespace bhlib {

TEMPLATE_TEST_CASE(
    "TypeDef", "", Sized<1>, Sized<2>, Sized<4>, Sized<8>, Sized<16>, Sized<32>, Sized<64>, Sized<128>, Sized<256>,
    Sized<512>, Sized<1'024>, Sized<2'048>
) {
    using ValueType = TestType;

    struct TestTag {};

    using TestTypeDef = TypeDef<TestType, TestTag>;
    CHECK(sizeof(TestTypeDef) == sizeof(ValueType));

    SECTION("Owns value on const") {
        Tester<const TestTypeDef, const ValueType> tester;
        const ValueType value = ValueType::generate_random();
        const TestTypeDef instance{value};
        tester.test(instance);
    }
    SECTION("Owns value on non const") {
        Tester<TestTypeDef, ValueType> tester;
        ValueType value = ValueType::generate_random();
        TestTypeDef instance{value};
        tester.test(instance);
    }
}
}  // namespace bhlib
