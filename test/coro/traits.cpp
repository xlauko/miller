
#include <doctest/doctest.h>

#include <vector>
#include <coroutine>

import miller.coro;

using namespace mi::coro;

TEST_CASE("awaiter concept") {
    struct awaitable_ready_missing { };

    struct awaitable_ready_broken {
        void await_ready() {};
        void await_suspend(std::coroutine_handle<>) {};
        void await_resume() {};
    };

    struct awaitable_suspend_missing {
        bool await_ready() { return false; };
        void await_resume() {};
    };

    struct wrong {};

    struct awaitable_suspend_broken {
        bool await_ready() { return false; };
        wrong await_suspend(std::coroutine_handle<>) { return {}; };
        void await_resume() {};
    };

    struct awaitable_resume_missing {
        bool await_ready() { return false; };
        void await_suspend(std::coroutine_handle<>) {};
    };

    struct awaitable {
        bool await_ready() { return false; };
        void await_suspend(std::coroutine_handle<>) {};
        void await_resume() {};
    };

    static_assert( !awaiter< awaitable_ready_missing > );
    static_assert( !awaiter< awaitable_ready_broken > );
    static_assert( !awaiter< awaitable_suspend_missing > );
    static_assert( !awaiter< awaitable_suspend_broken > );
    static_assert( !awaiter< awaitable_resume_missing > );
    static_assert( awaiter< awaitable > );

    struct awaitable_int {
        bool await_ready() { return false; };
        void await_suspend(std::coroutine_handle<>) {};
        int await_resume() { return 0; };
    };

    static_assert( std::is_same_v< void, await_result_t< awaitable > > );
    static_assert( std::is_same_v< int, await_result_t< awaitable_int > > );
}
