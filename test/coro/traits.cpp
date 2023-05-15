
#include <doctest/doctest.h>

#include <vector>
#include <coroutine>

import miller.coro;

TEST_CASE("is_coroutine_handle trait") {
    static_assert( !mi::is_coroutine_handle< int > );
    static_assert( mi::is_coroutine_handle< std::coroutine_handle< int > > );
    static_assert( mi::is_coroutine_handle< std::coroutine_handle< void > > );
    static_assert( mi::is_coroutine_handle< std::coroutine_handle<> > );
    static_assert( mi::is_coroutine_handle< std::coroutine_handle< std::noop_coroutine_promise > > );
}

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

    static_assert( !mi::awaiter< awaitable_ready_missing > );
    static_assert( !mi::awaiter< awaitable_ready_broken > );
    static_assert( !mi::awaiter< awaitable_suspend_missing > );
    static_assert( !mi::awaiter< awaitable_suspend_broken > );
    static_assert( !mi::awaiter< awaitable_resume_missing > );
    static_assert( mi::awaiter< awaitable > );

    struct awaitable_int {
        bool await_ready() { return false; };
        void await_suspend(std::coroutine_handle<>) {};
        int await_resume() { return 0; };
    };

    static_assert( std::is_same_v< void, mi::awaitable_traits< awaitable >::await_result_t > );
    static_assert( std::is_same_v< int, mi::awaitable_traits< awaitable_int >::await_result_t > );
}
