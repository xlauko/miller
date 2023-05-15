
#include <doctest/doctest.h>

#include <vector>
#include <coroutine>

import miller.coro;

namespace mi::test
{
    generator< int > iota() {
        int value = 0;
        while (true) {
            co_yield value;
            value++;
        }
    }

    TEST_CASE("tagged int") {
        int i = 0;
        for (int v : iota()) {
            CHECK_EQ(i++, v);
            if (i == 5)
                break;
        }
    }

    generator< int > empty_generator() {
        return {};
    }

    TEST_CASE("empty generator") {
        auto v = empty_generator();
    }

} // namespace mi::test
