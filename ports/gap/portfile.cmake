vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO lifting-bits/gap
  REF a374150133ed0f266d631a1fbf43b368553ab004
  SHA512 50f1406953fcfb96fc7a7596d490f83d9f545f6352adafffab296b66acce8d2e7d02c0b40187336b897dd26595ec888625cd09ff5baacb78ec9ba33cd2bb0e8e
  HEAD_REF main
)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS
    -DGAP_ENABLE_COROUTINES=OFF
    -DENABLE_TESTING=OFF
    -DGAP_INSTALL=ON
    -DUSE_SYSTEM_DEPENDENCIES=ON
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(
  PACKAGE_NAME "gap"
  CONFIG_PATH lib/cmake/gap
)

file( REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/" )

# we do not populate lib folder yet
file( REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/lib" )

file(
  INSTALL "${SOURCE_PATH}/LICENSE"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
  RENAME copyright
)

if ( EXISTS "${CMAKE_CURRENT_LIST_DIR}/${lower_package}_usage" )
  file(
    INSTALL "${CMAKE_CURRENT_LIST_DIR}/${lower_package}_usage"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${lower_package}"
    RENAME usage
  )
endif()
