add_executable( miller-lsp-server
    miller-lsp-server.cpp
)

find_package( argparse CONFIG REQUIRED )

target_link_libraries( miller-lsp-server
    PRIVATE
        argparse::argparse
        fmt::fmt
        mi::config
    INTERFACE
        miller_project_options
        miller_project_warnings
)
