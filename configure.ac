dnl Process this file with autoconf to produce a configure script.
AC_INIT([intel-hex-loader],[0.1.1])
AC_CONFIG_SRCDIR([intel-hex-loader.c])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
LT_INIT

dnl Checks for programs.
AC_PROG_CC

dnl Test scripts use `readlink`, so check for it
AC_CHECK_PROG(HAS_READLINK, readlink, yes, no)
if test x"$HAS_READLINK" != x"yes"
then
	AC_MSG_ERROR([The test suite requires the readlink program. It is part of GNU coreutils.])
fi

dnl Checks for header files.
AC_CHECK_HEADERS(errno.h getopt.h stdbool.h stdlib.h stdint.h stdio.h string.h, [], [
    AC_MSG_ERROR([Required standard header is missing.])
])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

