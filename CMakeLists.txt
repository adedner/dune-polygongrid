cmake_minimum_required( VERSION 2.8.12 )
project( dune-polygongrid CXX )

find_package( dune-common )
list( APPEND CMAKE_MODULE_PATH ${dune-common_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules" )

include( DuneMacros )
dune_project()

add_subdirectory( cmake/modules )
add_subdirectory( dune )
add_subdirectory( gridselector )
add_subdirectory( test )

finalize_dune_project( GENERATE_CONFIG_H_CMAKE )
