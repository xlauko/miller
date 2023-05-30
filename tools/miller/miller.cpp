#include <argparse/argparse.hpp>
#include <fmt/imp.h>

import miller.config;

namespace mi {

    using options_config = argparse::ArgumentParser;

    options_config get_options_config() {
        options_config config("miller", mi::miller_version);

        config.add_argument("program")
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

} // namespace mi


int main(int argc, char* argv[]) try {
    auto opts = mi::get_options_config();
    opts.parse_args(argc, argv);

} catch (const std::runtime_error& err) {
    fmt::print(stderr, "{}\n", err.what());
    return 1;
}
