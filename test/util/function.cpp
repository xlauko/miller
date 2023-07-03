#include <doctest/doctest.h>

import miller.util;

namespace mi::test
{
    constexpr auto call = [] (function_ref< int() > fn) {
        return fn();
    };

    int f() { return 1; }

    template< typename type >
    type g() {
        return {};
    }

    struct object_t {
        int operator()() { return 3; }
        int operator()() const { return 4; }
    };

    TEST_SUITE("util::function::basic") {
        TEST_CASE("free function") {
            CHECK_EQ(call(f), f());
        }

        TEST_CASE("closure") {
            CHECK_EQ(call([] { return 4; }), 4);
        }

        TEST_CASE("closure with value capture") {
            CHECK_EQ(call([x = 4] { return x; }), 4);
        }

        TEST_CASE("closure with reference capture") {
            int x = 4;
            CHECK_EQ(call([&] { return x; }), 4);
        }

        TEST_CASE("template specialization") {
            CHECK_EQ(call(g< int >), int{});
        }

        TEST_CASE("mutable obejct") {
            object_t obj;
            CHECK_EQ(call(obj), 3);
        }

        TEST_CASE("immutable obejct") {
            const object_t obj;
            CHECK_EQ(call(obj), 4);
        }
    }

} // namespace mi::test
