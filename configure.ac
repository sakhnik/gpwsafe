AC_INIT([gpwsafe],[1.0],[sakhnik@gmail.com])
AC_PREREQ(2.59)
dnl AC_CONFIG_AUX_DIR(config)

m4_include([m4/ax_boost_base.m4])

AM_INIT_AUTOMAKE([1.9])

AC_PROG_CXX

AM_PATH_LIBGCRYPT([1.2.0],[],[])
AX_BOOST_BASE([1.40],[],[])

AC_OUTPUT([Makefile src/Makefile])
