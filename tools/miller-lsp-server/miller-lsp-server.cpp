#include <argparse/argparse.hpp>
#include <fmt/core.h>

#include <refl.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/cfg/argv.h>

#include <coroutine>

import miller.config;

namespace mi {

    using options_config = argparse::ArgumentParser;

    options_config get_options_config() {
        options_config config("miller-lsp-server", mi::miller_version);

        config.add_argument("--pretty")
            .help("Pretty-print JSON output")
            .default_value(false)
            .implicit_value(true);

        return config;
    }

} // namespace mi

int main(int argc, char* argv[]) try {
    auto opts = mi::get_options_config();
    opts.parse_args(argc, argv);

    spdlog::cfg::load_env_levels();
    spdlog::cfg::load_argv_levels(argc, argv);

} catch (const std::runtime_error& err) {
    fmt::print(stderr, "{}\n", err.what());
    return 1;
}
