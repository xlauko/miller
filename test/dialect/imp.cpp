
#include <coroutine>
#include <forward_list>
#include <iostream>
#include <variant>
#include <vector>

#include <doctest/doctest.h>

#include <spdlog/spdlog.h>

import miller.dialect.imp;
import miller.program;
import miller.util;

using namespace mi::imp;

namespace mi::test
{
    TEST_SUITE("mi::imp") {
        TEST_CASE("empty program") {
            imp::program p{};
            CHECK(p.empty());

            CHECK_EQ( entry(p), entry(p.body) );
            CHECK_EQ( entry(p), exit(p) );
            CHECK_EQ( entry(p), entry(p.body) );
            CHECK_EQ( entry(p.body), exit(p.body) );

            CHECK( !p.escape() );
        }

        TEST_CASE("variable declaration") {
            imp::program p(assign({"v"}, constant(4u)));
            CHECK(std::holds_alternative< assign >( p.front() ));

            CHECK_EQ( entry(p), entry(p.body) );
            CHECK_EQ( entry(p.body), entry(p.body.front()) );

            CHECK_EQ( exit(p), exit(p.body) );

            CHECK( !p.escape() );
        }

        TEST_CASE("arithmetic computation") {
            imp::program p(
                assign({"a"}, constant(1u)),
                assign({"b"},
                    make_arithmetic< arithmetic_kind::add >(
                        variable("a"), constant(2u)
                    )
                ),
                assign({"c"},
                    make_arithmetic< arithmetic_kind::mul >(
                        variable("b"), constant(3u)
                    )
                )
            );

            CHECK(std::holds_alternative< assign >( p.front() ));

            CHECK_EQ( entry(p), entry(p.body) );
            CHECK_EQ( entry(p.body), entry(p.body.front()) );

            CHECK_EQ( exit(p), exit(p.body) );

            const auto &vara = p.body[0];
            const auto &varb = p.body[1];
            const auto &varc = p.body[2];

            CHECK_EQ( exit_of(p, vara).value(), entry(varb) );
            CHECK_EQ( exit_of(p, varb).value(), entry(varc) );

            CHECK_EQ( exit_of(p, varc).value(), exit(p) );
            CHECK_EQ( exit_of(p, varc).value(), exit(p.body) );

            CHECK( !p.escape() );
        }

        TEST_CASE("condition in program") {
            imp::program p(
                conditional(
                    make_relational< predicate::eq >(variable("v"),  constant(0u)),
                    terminate(),
                    skip()
                ),
                assign({"v"}, constant(1u))
            );

            const auto &cond   = unwrap< conditional >( p.front() );
            const auto &assign_stmt = p.back();
            const auto &front = p.front();


            CHECK_EQ( exit(p), exit_of(p, assign_stmt).value() );
            CHECK_EQ( exit_of(p, front).value(), entry(assign_stmt) );
            CHECK_EQ( exit_of(p, front).value(), exit_of(p, cond.then_stmt).value() );
            CHECK_EQ( exit_of(p, front).value(), exit_of(p, cond.else_stmt).value() );

            CHECK( !p.escape() );
        }

        TEST_CASE("condition at end of program") {
            imp::program p(
                conditional(
                    make_relational< predicate::eq >(variable("v"),  constant(0u)),
                    terminate(),
                    skip()
                )
            );

            CHECK_EQ( entry(p), entry(p.body) );
            CHECK_EQ( entry(p.body), entry(p.body.front()) );

            CHECK_EQ( exit(p), exit(p.body) );

            const auto &cond_stmt = p.front();

            CHECK_EQ( exit(p), exit_of(p, cond_stmt).value() );

            CHECK( !p.escape() );
        }

        TEST_CASE("loop in program") {
            imp::program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    skip()
                )
            );

            CHECK_EQ( entry(p), entry(p.body) );
            CHECK_EQ( entry(p.body), entry(p.body.front()) );

            CHECK_EQ( exit(p), exit(p.body) );

            const auto &front = p.front();
            const auto &loop = unwrap< while_loop >( front );

            CHECK_EQ( exit(p), exit_of(p, front).value() );
            CHECK_EQ( exit_of(p, front).value(), exit_of(p, loop.body).value() );
            CHECK_EQ( exit_of(p, front).value(), exit_of(p, loop.body.back()).value() );

            CHECK( !p.escape() );
        }

        TEST_CASE("nested scopes") {
            imp::program p{
                scope(skip()), skip()
            };

            CHECK_EQ( entry(p), entry(p.body) );
            CHECK_EQ( entry(p.body), entry(p.body.front()) );

            CHECK_EQ( exit(p), exit(p.body) );

            const auto &front = p.front();
            const auto &sc = unwrap< scope >( front );

            CHECK_EQ( entry(p), entry(front) );
            CHECK_EQ( entry(p), entry(sc.front()) );

            CHECK_EQ( exit_of(p, front).value(), entry(p.back()) );
            CHECK_EQ( exit_of(p, sc.front()).value(), entry(p.back()) );


            CHECK( !p.escape() );
        }

        TEST_CASE("nested scopes reversed") {
            imp::program p{
                skip(), scope(skip())
            };

            CHECK_EQ( entry(p), entry(p.body) );
            CHECK_EQ( entry(p.body), entry(p.body.front()) );

            CHECK_EQ( exit(p), exit(p.body) );

            const auto &back = p.back();
            const auto &sc = unwrap< scope >( back );

            CHECK_EQ( entry(p), entry(p.front()) );
            CHECK_EQ( exit(p), exit_of(p, back).value() );
            CHECK_EQ( exit(p), exit_of(p, sc.front()).value() );

            CHECK( !p.escape() );
        }

        TEST_CASE("sequenced scopes") {
            imp::program p(
                (scope(skip())), (scope(skip()))
            );

            CHECK_EQ( entry(p), entry(p.body) );
            CHECK_EQ( entry(p.body), entry(p.body.front()) );
            CHECK_EQ( exit(p), exit(p.body) );

            CHECK( !p.escape() );
        }

        TEST_CASE("escape complex nested scopes") {
            imp::program p(
                (scope(scope(break_iteration()), skip()))
            );


            CHECK( p.escape() );
        }

        TEST_CASE("escape nested scopes first") {
            imp::program p{
                scope(skip()), scope(break_iteration())
            };

            CHECK( p.escape() );
        }

        TEST_CASE("escape nested scopes second") {
            imp::program p(
                (scope(break_iteration())), (scope(skip()))
            );

            CHECK( p.escape() );
        }

        TEST_CASE("escape while") {
            imp::program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    conditional(
                        make_relational< predicate::eq >(variable("v"),  constant(0u)),
                        break_iteration(),
                        skip()
                    )
                )
            );

            const auto &loop = unwrap< while_loop >(  p.front() );
            CHECK( loop.body.escape() );
        }

        TEST_CASE("escape nested inner while") {
            imp::program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    while_loop(
                        make_relational< predicate::eq >(variable("v"),  constant(0u)),
                        break_iteration()
                    )
                )
            );

            CHECK( !p.escape() );

            const auto &loop = unwrap< while_loop >(  p.front() );
            CHECK( !loop.escape() );

            CHECK( !loop.body.escape() );

            const auto &inner = unwrap< while_loop >( loop.body.back() );
            CHECK( !inner.escape() );
            CHECK( inner.body.escape() );
        }

        TEST_CASE("escape nested outer while") {
            imp::program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    while_loop(
                        make_relational< predicate::eq >(variable("v"),  constant(0u)),
                        skip()
                    ),
                    break_iteration()
                )
            );

            CHECK( !p.escape() );

            const auto &loop = unwrap< while_loop >(  p.front() );
            CHECK( !loop.escape() );
            CHECK( loop.body.escape() );

            const auto &inner = unwrap< while_loop >( loop.body.front() );
            CHECK( !inner.escape() );
        }

        static_assert( operation< imp::program > );
        static_assert( operation< imp::scope > );
        static_assert( operation< imp::while_loop > );
        static_assert( operation< imp::conditional > );
        static_assert( operation< imp::skip > );
        static_assert( operation< imp::break_iteration > );
        static_assert( operation< imp::terminate > );
        static_assert( operation< imp::assign > );

    } // test suite imp dialect

} // namespace mi::test
