
#include <coroutine>
#include <forward_list>
#include <iostream>
#include <variant>
#include <vector>

#include <doctest/doctest.h>
#include <refl.hpp>
#include <spdlog/spdlog.h>

import miller.dialects;
import miller.program;
import miller.util;

using namespace mi::imp;

namespace mi::test
{
    TEST_SUITE("mi::imp") {
        TEST_CASE("empty program") {
            imp::program p{};
            CHECK(p.empty());

            CHECK_EQ( p.entry(), p.exit() );
            CHECK_EQ( p.entry(), p.exit() );

            CHECK( !p.escape() );
        }

        TEST_CASE("variable declaration") {
            imp::program p(assign({"v"}, constant(4u)));
            CHECK( p.front().isa< assign >() );

            CHECK_EQ( p.entry(), p.front().entry() );
            CHECK_EQ( p.exit(), p.exit() );

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

            CHECK( p.front().isa< assign >() );

            CHECK_EQ( p.entry(), p.front().entry() );

            const auto &vara = p.body[0];
            const auto &varb = p.body[1];
            const auto &varc = p.body[2];

            CHECK_EQ( p.exit_of(vara).value(), varb.entry() );
            CHECK_EQ( p.exit_of(varb).value(), varc.entry() );
            CHECK_EQ( p.exit_of(varc).value(), p.exit() );
            CHECK_EQ( p.exit_of(varc).value(), p.exit() );

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

            const auto &cond = p.front().unwrap< conditional >();
            const auto &assign_stmt = p.back();
            const auto &front = p.front();

            CHECK_EQ( p.exit(), p.exit_of(assign_stmt).value() );
            CHECK_EQ( p.exit_of(front).value(), assign_stmt.entry() );
            CHECK_EQ( p.exit_of(front).value(), p.exit_of(cond.then_stmt).value() );
            CHECK_EQ( p.exit_of(front).value(), p.exit_of(cond.else_stmt).value() );

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

            CHECK_EQ( p.entry(), p.entry() );
            CHECK_EQ( p.entry(), p.front().entry() );

            CHECK_EQ( p.exit(), p.exit() );

            const auto &cond_stmt = p.front();

            CHECK_EQ( p.exit(), p.exit_of(cond_stmt).value() );

            CHECK( !p.escape() );
        }

        TEST_CASE("loop in program") {
            imp::program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    scope(
                        skip(), skip()
                    )
                )
            );

            CHECK_EQ( p.entry(), p.entry() );
            CHECK_EQ( p.entry(), p.front().entry() );

            CHECK_EQ( p.exit(), p.exit() );

            const auto &front = p.front();
            const auto &loop = front.unwrap< while_loop >();

            CHECK_EQ( p.exit(), p.exit_of(front).value() );
            CHECK_EQ( p.exit_of(front).value(), p.exit_of(loop.body).value() );

            const auto &body = loop.body.unwrap< scope >();
            CHECK_EQ( p.exit_of(front).value(), p.exit_of(body.back()).value() );

            CHECK( !p.escape() );
        }

        TEST_CASE("nested scopes") {
            imp::program p{
                scope(skip()), skip()
            };

            CHECK_EQ( p.entry(), p.entry() );
            CHECK_EQ( p.entry(), p.front().entry() );

            CHECK_EQ( p.exit(), p.exit() );

            const auto &front = p.front();
            const auto &sc = front.unwrap< scope >();

            CHECK_EQ( p.entry(), front.entry() );
            CHECK_EQ( p.entry(), sc.front().entry() );

            CHECK_EQ( p.exit_of(front).value(), p.back().entry() );
            CHECK_EQ( p.exit_of(sc.front()).value(), p.back().entry() );

            CHECK( !p.escape() );
        }

        TEST_CASE("nested scopes reversed") {
            imp::program p{
                skip(), scope(skip())
            };

            CHECK_EQ( p.entry(), p.entry() );
            CHECK_EQ( p.entry(), p.front().entry() );

            CHECK_EQ( p.exit(), p.exit() );

            const auto &sc = p.back().unwrap< scope >();

            CHECK_EQ( p.entry(), p.front().entry() );
            CHECK_EQ( p.exit(), p.exit_of(p.back()).value() );
            CHECK_EQ( p.exit(), p.exit_of(sc.front()).value() );

            CHECK( !p.escape() );
        }

        TEST_CASE("sequenced scopes") {
            imp::program p(
                (scope(skip())), (scope(skip()))
            );

            CHECK_EQ( p.entry(), p.entry() );
            CHECK_EQ( p.entry(), p.front().entry() );
            CHECK_EQ( p.exit(), p.exit() );

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

            const auto &loop = p.front().unwrap< while_loop >();
            CHECK( !loop.escape() );
            CHECK( loop.body.escape() );
        }

        TEST_CASE("escape nested inner while") {
            imp::program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    scope(while_loop(
                        make_relational< predicate::eq >(variable("v"),  constant(0u)),
                        scope(break_iteration())
                    ))
                )
            );

            CHECK( !p.escape() );

            const auto &loop = p.front().unwrap< while_loop >();
            CHECK( !loop.escape() );

            CHECK( !loop.escape() );

            const auto &body = loop.body.unwrap< scope >();
            const auto &inner = body.back().unwrap< while_loop >();
            CHECK( !inner.escape() );
            CHECK( inner.body.escape() );
        }

        TEST_CASE("escape nested outer while") {
            imp::program p(
                while_loop(
                    make_relational< predicate::gt >(variable("v"),  constant(0u)),
                    scope(while_loop(
                        make_relational< predicate::eq >(variable("v"),  constant(0u)),
                        break_iteration()
                    ),
                    break_iteration())
                )
            );

            CHECK( !p.escape() );

            const auto &loop = p.front().unwrap< while_loop >();
            CHECK( !loop.escape() );

            const auto &body = loop.body.unwrap< scope >();
            CHECK( body.escape() );
            const auto &inner = body.front().unwrap< while_loop >();
            CHECK( !inner.escape() );
        }

        static_assert( operation_like< imp::program > );
        static_assert( operation_like< imp::while_loop > );
        static_assert( operation_like< imp::conditional > );
        static_assert( operation_like< imp::skip > );
        static_assert( operation_like< imp::break_iteration > );
        static_assert( operation_like< imp::terminate > );
        static_assert( operation_like< imp::assign > );

    } // test suite imp dialect

} // namespace mi::test
