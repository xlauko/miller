#include <doctest/doctest.h>

#include <vector>
#include <coroutine>

import miller.coro;

using namespace mi::coro;

struct awaitable_type {
    bool await_ready() { return false; };
    void await_suspend(std::coroutine_handle<>) {};
    void await_resume() {};
};

struct int_awaitable_type {
    bool await_ready() { return false; };
    void await_suspend(std::coroutine_handle<>) {};
    int await_resume() { return 0; };
};

using zero_value_func = decltype([] { return 0; });
using identity = decltype([] (auto v) { return v; });

TEST_CASE("fmap awaiter") {
    static_assert( awaiter<
        fmap_awaiter< zero_value_func, awaitable_type >
    >);

    static_assert( awaiter<
        fmap_awaiter< identity, int_awaitable_type >
    >);
}
