
#include <coroutine>
#include <forward_list>
#include <doctest/doctest.h>
#include <variant>
#include <vector>

import miller.dialect.core;
import miller.program;

using namespace mi::core;

namespace mi::test
{
    TEST_SUITE("mi::core") {
        TEST_CASE("empty program") {
            core::program p;
            CHECK(p.empty());
        }

        TEST_CASE("variable declaration") {
            core::program p(assign({"v"}, constant(4u)));
            CHECK(std::holds_alternative< assign >( p.front() ));
        }

        TEST_CASE("conditions in program") {
            core::program p(
                conditional(
                    make_relational< predicate::eq >(variable("v"),  constant(0u)),
                    terminate_stmt,
                    skip_stmt
                )
            );
        }

        TEST_CASE("loop in program") {
            core::program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    skip_stmt
                )
            );
        }

        static_assert( operation< core::program > );
        static_assert( operation< core::compound > );
        static_assert( operation< core::while_loop > );
        static_assert( operation< core::conditional > );
        static_assert( operation< core::skip > );
        static_assert( operation< core::break_loop > );
        static_assert( operation< core::terminate > );
        static_assert( operation< core::assign > );

    } // test suite core dialects



} // namespace mi::test
