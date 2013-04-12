set(TESTS test-polygongrid)

foreach(_executable ${TESTS})
  add_executable(${_executable} EXCLUDE_FROM_ALL ${_executable}.cc)
  target_link_libraries(${_executable} ${DUNE_LIBS})
  add_test(${_executable} ${_executable})
  add_dune_mpi_flags(${_executable})
endforeach(_executable ${TESTS})

add_directory_test_target(test_target)
add_dependencies(${test_target} ${TESTS}) 
