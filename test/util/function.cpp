#include <doctest/doctest.h>

import miller.util;

namespace mi::test
{
    constexpr auto call = [] (mi::function_ref< int() > fn) {
        return fn();
    };

    constexpr auto callc = [] (mi::function_ref< int() const > fn) {
        return fn();
    };

    constexpr auto callcn = [] (mi::function_ref< int() const noexcept > fn) {
        return fn();
    };

    int one() { return 1; }

    template< typename type >
    type def() {
        return {};
    }

    constexpr int nonconst_int = 3;
    constexpr int const_int = 4;

    struct object_t {
        int operator()() { return nonconst_int; }
        int operator()() const { return const_int; }
    };

    TEST_SUITE("util::function::basic") {
        TEST_CASE("free function") {
            CHECK_EQ(call(one), one());
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
            CHECK_EQ(call(def< int >), int{});
        }

        TEST_CASE("mutable obejct") {
            object_t obj;
            CHECK_EQ(call(obj), nonconst_int);
        }

        TEST_CASE("immutable obejct") {
            const object_t obj;
            CHECK_EQ(call(obj), const_int);
        }
    } // test suite function ref basic

    TEST_SUITE("util::function::const") {
        TEST_CASE("callable as const") {
            SUBCASE("closure") {
                CHECK_EQ(callc([] { return 1; }), 1);
            }

            SUBCASE("mutable object") {
                object_t obj;
                CHECK_EQ(callc(obj), const_int);
            }

            SUBCASE("immutable object") {
                object_t const obj;
                CHECK_EQ(callc(obj), const_int);
            }

            SUBCASE("prvalue") {
                CHECK_EQ(callc(object_t{}), const_int);
            }

            SUBCASE("const xvalue") {
                object_t const obj;
                CHECK_EQ(callc(static_cast< object_t const && >(obj)), const_int);
            }
        }

        struct mutable_object {
            int operator()() { return nonconst_int; }
        };

        static_assert(not std::is_invocable_v<
            decltype(callc),
            decltype([i = 0]() mutable { return i; })
        >, "const-qualified signature cannot reference mutable lambda");

        static_assert(std::is_invocable_v<decltype(call), mutable_object>);
        static_assert(std::is_invocable_v<decltype(call), mutable_object &>);
        static_assert(std::is_invocable_v<decltype(call), mutable_object &&>);
        static_assert(not std::is_invocable_v<decltype(call), mutable_object const &>,
            "const object is not callable");
        static_assert(not std::is_invocable_v<decltype(call), mutable_object const &&>);

        static_assert(not std::is_invocable_v<decltype(callc), mutable_object>,
            "const-qualified rvalue is not callable");

        static_assert(not std::is_invocable_v<decltype(callc), mutable_object &>,
            "const-qualified lvalue is not callable");

        static_assert(not std::is_invocable_v<decltype(callc), mutable_object &&>);
        static_assert(not std::is_invocable_v<decltype(callc), mutable_object const &>);
        static_assert(not std::is_invocable_v<decltype(callc), mutable_object const &&>);

    } // test suite function ref const

    TEST_SUITE("util::function::noexcept") {

        struct noexcept_object {
            int operator()() noexcept { return 11; }
            int operator()() const noexcept { return 22; }
        };

        TEST_CASE("callable is const noexcept") {
            noexcept_object obj;
            noexcept_object const cobj;

            SUBCASE("unqualified signature call non-const object") {
                CHECK_EQ(call(obj), 11);
            }

            SUBCASE("qualified signature call non-const object") {
                CHECK_EQ(callcn(obj), 22);
            }

            SUBCASE("unqualified signature call const object") {
                CHECK_EQ(call(cobj), 22);
            }

            SUBCASE("qualified signature call const object") {
                CHECK_EQ(callcn(cobj), 22);
            }
        }

        TEST_CASE("noexcept closure") {
            CHECK_EQ(call([] () noexcept { return 10; }), 10);
        }

        static_assert(not std::is_invocable_v<
            decltype(callcn),
            decltype([] { return 0; })
        >, "noexcept signature does not accept potentially-throwing lambda");

        static_assert(not std::is_invocable_v<
            decltype(callcn),
            decltype([i = 0]() mutable noexcept { return i; })
        >, "const noexcept signature does not accept noexcept but mutable lambda");

    } // test suite function ref noexcept

} // namespace mi::test
