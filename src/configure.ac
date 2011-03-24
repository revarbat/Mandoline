#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([Mandoline], 1.0, [revarbat@gmail.com])
AC_PREREQ(2.59)
AM_INIT_AUTOMAKE([1.9.6 no-define foreign])
AC_CONFIG_HEADERS([config.h])
# AC_CONFIG_SRCDIR([CarvedSlice.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([inttypes.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_HEADER_TIME
AC_C_VOLATILE

# allow for a variety of compile CFLAGS to be passed to gcc
AS_IF([test ".$CFLAGS"="."], [ CFLAGS="" ])
AC_ARG_WITH([warn],
    AS_HELP_STRING(--with-warn, use -Wall preproc flag),
    [CFLAGS="$CFLAGS -Wall"], [CFLAGS="$CFLAGS -Wall"])
AC_ARG_WITH([opt],
    AS_HELP_STRING(--with-opt=OPT, Specify optimization),
    [CFLAGS="$CFLAGS -${withval}"],
    [CFLAGS="$CFLAGS -O3"])
AC_ARG_WITH([gprof],
    AS_HELP_STRING(--with-gprof, use -pg preproc flag),
    [CFLAGS="$CFLAGS -pg"])

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday])

AC_CHECK_DECLS([optreset],,, [[#include <getopt.h>]])

# Checks for pthreads
ACX_PTHREAD(,)
LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"

AC_CONFIG_FILES([Makefile])
# AC_CONFIG_SUBDIRS([BGL])
AC_OUTPUT

