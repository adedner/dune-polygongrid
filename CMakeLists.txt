cmake_minimum_required(VERSION 3.16)
project(dune-polygongrid CXX)

find_package(dune-common)
list(APPEND CMAKE_MODULE_PATH ${dune-common_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

include(DuneMacros)

# deactivate global include-directories
dune_policy(SET DP_DEFAULT_INCLUDE_DIRS NEW)

# deactivate global calls to add_dune_all_flags in tests
dune_policy(SET DP_TEST_ADD_ALL_FLAGS NEW)

# start a dune project with information from dune.module
dune_project()

# add interface library
dune_add_library(dunepolygongrid EXPORT_NAME PolygonGrid NAMESPACE Dune::)

# enable everything
dune_default_include_directories(dunepolygongrid PUBLIC)
link_libraries(Dune::PolygonGrid)
dune_enable_all_packages()

add_subdirectory(cmake/modules)
add_subdirectory(dune)
add_subdirectory(test)

# if Python bindings are enabled, include necessary sub directories.
if( DUNE_ENABLE_PYTHONBINDINGS )
  add_subdirectory(python)
endif()

finalize_dune_project(GENERATE_CONFIG_H_CMAKE)
