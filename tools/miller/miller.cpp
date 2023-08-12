#include <argparse/argparse.hpp>
#include <fmt/core.h>

#include <refl.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/cfg/argv.h>

#include <coroutine>

import miller.analysis;
import miller.config;
import miller.dialects;
import miller.program;
import miller.domains;

namespace mi {

    using options_config = argparse::ArgumentParser;

    namespace opts {
        constexpr std::string_view program = "program";
    };

    options_config get_options_config() {
        options_config config("miller", mi::miller_version);

        config.add_argument( opts::program )
            .required()
            .help("mlir program to process");

        config.add_argument("--verbose")
            .default_value(false)
            .implicit_value(true);

        config.add_argument("-o", "--output")
            .default_value(std::string("-"))
            .required()
            .help("specify the output file.");

        return config;
    }

    auto test_program() {
        using namespace imp;

        return mi::imp::program(
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
    }

} // namespace mi

int main(int argc, char* argv[]) try {
    auto opts = mi::get_options_config();
    opts.parse_args(argc, argv);

    spdlog::cfg::load_env_levels();
    spdlog::cfg::load_argv_levels(argc, argv);

    //    auto p = mi::program::load( opts.get(mi::opts::program) );

    // using domain = mi::domains::environment< std::string, mi::domains::vars >;
    // auto result = mi::analysis::forward_fixpoint< domain >(p);

} catch (const std::runtime_error& err) {
    fmt::print(stderr, "{}\n", err.what());
    return 1;
}
