#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([restStatServer], [0.1.0], [artraf@yandex.com])
AC_CONFIG_SRCDIR([src/restserver.cpp])
AC_CONFIG_HEADERS([include/config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_INIT_AUTOMAKE
AX_CXX_COMPILE_STDCXX_14

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpistache':
AC_CHECK_LIB([pistache], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([sysinfo])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT