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
    nlohmann_json::nlohmann_json
)

target_sources( mi-lsp
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      error.ccm
      lsp.ccm
      protocol.ccm
      server.ccm
)

add_library( mi::lsp ALIAS mi-lsp )
