add_directory_test_target( alltests )

foreach( test test-polygongrid )
  add_executable( ${test} EXCLUDE_FROM_ALL ${test}.cc)
  target_link_dune_default_libraries( ${test} )
  add_dune_mpi_flags( ${test} )

  add_test( ${test} ${test} )
  add_dependencies( ${alltests} ${test} )
endforeach()
