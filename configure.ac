AC_PREREQ([2.62])
DUNE_AC_INIT
AC_CONFIG_SRCDIR([dune/grid/polygongrid.hh])
AM_CONFIG_HEADER([config.h])


DUNE_CHECK_ALL

AC_SUBST(AM_CPPFLAGS, $DUNE_CPPFLAGS)
AC_SUBST(AM_LDFLAGS, $DUNE_LDFLAGS)
LIBS="$DUNE_LIBS"

AC_CONFIG_FILES([
  Makefile
  cmake/Makefile
  cmake/modules/Makefile
  cmake/pkg/Makefile
  dune/Makefile
  dune/grid/Makefile
  dune/grid/polygongrid/Makefile
  dune/grid/test/Makefile
  m4/Makefile
  polygongrid/Makefile
  dune-polygongrid.pc
])

AC_OUTPUT
DUNE_SUMMARY_ALL
