#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/cfg/argv.h>

int main(int argc, char** argv) {
    spdlog::cfg::load_env_levels();
    spdlog::cfg::load_argv_levels(argc, argv);

    doctest::Context context(argc, argv);

    return context.run();
}
