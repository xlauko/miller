
#include <coroutine>
#include <forward_list>
#include <doctest/doctest.h>
#include <variant>
#include <vector>

import miller.dialect.core;

using namespace mi::core;

namespace mi::test
{
    TEST_SUITE("mi::core") {
        TEST_CASE("empty program") {
            program p;
            CHECK(p.empty());
        }

        TEST_CASE("variable declaration") {
            program p(assign({"v"}, constant(4u)));
            CHECK(std::holds_alternative< assign >( p.front() ));
        }

        TEST_CASE("conditions in program") {
            program p(
                conditional(
                    make_relational< predicate::eq >(variable("v"),  constant(0u)),
                    exit_stmt,
                    skip_stmt
                )
            );
        }

        TEST_CASE("loop in program") {
            program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    skip_stmt
                )
            );
        }
    } // test suite core dialects

} // namespace mi::test
