add_subdirectory( miller)

#
# miller config module
#
add_library( mi-config )

target_sources( mi-config
    PUBLIC
        FILE_SET miller_config_header_units
        TYPE CXX_MODULES
        FILES config.cpp
)

add_library( mi::config ALIAS mi-config )
