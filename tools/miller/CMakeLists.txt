add_executable( miller
    miller.cpp
)

find_package( argparse CONFIG REQUIRED )

target_link_libraries( miller
    PRIVATE
        argparse::argparse
        fmt::fmt
        mi::analysis
        mi::config
        mi::dialects
        mi::sema
    INTERFACE
        miller_project_options
        miller_project_warnings
)
