#
# miller language server protocol
#
add_library( mi-lsp )

target_link_libraries( mi-lsp
  PUBLIC
    mi::coro
    mi::util
  INTERFACE
    miller_project_options
    miller_project_warnings
  PRIVATE
    spdlog::spdlog
    spdlog::spdlog_header_only
    refl-cpp
)

target_sources( mi-lsp
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      server.mpp
)

add_library( mi::lsp ALIAS mi-lsp )
