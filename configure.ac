AC_INIT([gpwsafe],[1.0],[sakhnik@gmail.com])
AC_PREREQ(2.59)
dnl AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([1.9 color-tests])

AM_CONFIG_HEADER([config.h])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

AC_CHECK_FUNCS(getopt_long)
AC_CHECK_LIB(readline,readline,[],[AC_MSG_ERROR(cannot find libreadline)])

AM_PATH_LIBGCRYPT([1.2.0],[],[])

m4_include([m4/ax_boost_base.m4])
AX_BOOST_BASE([1.40],[],[])
m4_include([m4/ax_boost_unit_test_framework.m4])
AX_BOOST_UNIT_TEST_FRAMEWORK

AC_OUTPUT([Makefile \
           src/Makefile \
           test/Makefile])
