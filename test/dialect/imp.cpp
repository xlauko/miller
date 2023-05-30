
#include <coroutine>
#include <forward_list>
#include <doctest/doctest.h>
#include <variant>
#include <vector>

import miller.dialect.imp;
import miller.program;

using namespace mi::imp;

namespace mi::test
{
    TEST_SUITE("mi::imp") {
        TEST_CASE("empty program") {
            imp::program p;
            CHECK(p.empty());
        }

        TEST_CASE("variable declaration") {
            imp::program p(assign({"v"}, constant(4u)));
            CHECK(std::holds_alternative< assign >( p.front() ));
        }

        TEST_CASE("conditions in program") {
            imp::program p(
                conditional(
                    make_relational< predicate::eq >(variable("v"),  constant(0u)),
                    terminate_stmt,
                    skip_stmt
                )
            );
        }

        TEST_CASE("loop in program") {
            imp::program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    skip_stmt
                )
            );
        }

        static_assert( operation< imp::program > );
        static_assert( operation< imp::compound > );
        static_assert( operation< imp::while_loop > );
        static_assert( operation< imp::conditional > );
        static_assert( operation< imp::skip > );
        static_assert( operation< imp::break_loop > );
        static_assert( operation< imp::terminate > );
        static_assert( operation< imp::assign > );

    } // test suite imp dialects



} // namespace mi::test
