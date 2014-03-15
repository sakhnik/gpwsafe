dnl This file is part of gpwsafe.
dnl
dnl Gpwsafe is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl Gpwsafe is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with gpwsafe.  If not, see <http://www.gnu.org/licenses/>

AC_INIT([gpwsafe],[0.9.0],[sakhnik@gmail.com])
AC_PREREQ(2.59)

AM_INIT_AUTOMAKE([1.9 color-tests])
AM_SILENT_RULES([yes])

AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_LANG_PUSH([C++])
m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
AX_CXX_COMPILE_STDCXX_11([noext])
AC_LANG_POP
AC_PROG_RANLIB

AC_CHECK_FUNCS(getopt_long)
AC_CHECK_LIB(readline,readline,[],[AC_MSG_ERROR(cannot find libreadline)])

AM_PATH_LIBGCRYPT([1.2.0],[],[])

m4_include([m4/ax_boost_base.m4])
AX_BOOST_BASE([1.48],[],[])
m4_include([m4/ax_boost_program_options.m4])
AX_BOOST_PROGRAM_OPTIONS
m4_include([m4/ax_boost_unit_test_framework.m4])
AX_BOOST_UNIT_TEST_FRAMEWORK
dnl There's a bug in boost 1.50 preventing the memory pool
dnl to be used without linking, unless:
BOOST_CPPFLAGS=$BOOST_CPPFLAGS" -DBOOST_DISABLE_THREADS"

dnl --------------------------------------------------------------------------
dnl X clipboard support
AC_ARG_ENABLE(
	[xclip],
	AS_HELP_STRING([--enable-xclip=[yes]],
	               [Enable copying to the X clipboard (yes,no,gtk2,gtk3)])
)

AC_MSG_CHECKING(--enable-xclip argument)
AS_CASE([$enable_xclip],
	[no],    [AC_MSG_RESULT(no X clipboard support)],
	[yes|""],[AC_MSG_RESULT(yes - automatic X clipboard support)
	          enable_xclip=yes],
	[gtk*],  [AC_MSG_RESULT([yes - using $enable_xclip])],
	[AC_MSG_RESULT([Sorry, $enable_xclip X clipboard is not supported])]
)

AS_IF([test "x$enable_xclip" = "xgtk3"],[
	PKG_CHECK_MODULES([GTK],
	                  [gtk+-3.0],
	                  [gtk_found=yes])
])

AS_IF([test "x$enable_xclip" = "xgtk2"],[
	PKG_CHECK_MODULES([GTK],
	                  [gtk+-2.0],
	                  [gtk_found=yes])
])

AS_IF([test "x$enable_xclip" = "xyes"],[
	PKG_CHECK_MODULES([GTK],
	                  [gtk+-3.0],
	                  [gtk_found=yes enable_xclip=gtk3],
	                  [gtk_found=no])
])

AS_IF([test "x$enable_xclip" = "xyes"],[
	PKG_CHECK_MODULES([GTK],
	                  [gtk+-2.0],
	                  [gtk_found=yes enable_xclip=gtk2],
	                  [gtk_found=no])
])

AS_IF([test "x$gtk_found" = "xyes"],[
	AC_DEFINE([ENABLE_GTK],[1],[Define to compile with GTK+ support])
	AC_DEFINE([ENABLE_XCLIP],[1],[Define to compile with X clipboard support])
])

AM_CONDITIONAL([ENABLE_GTK],[test "x$gtk_found" = "xyes"])

AS_IF([test "x$enable_xclip" = "xyes"],[
	AC_MSG_ERROR([No suitable X clipboard support library found])
])

AM_CONDITIONAL([ENABLE_XCLIP],[test "x$enable_xclip" != "xno"])


dnl --------------------------------------------------------------------------
dnl Perl modules

m4_include([m4/ax_prog_perl_modules.m4])
AX_PROG_PERL_MODULES(Expect, [have_perl_expect=yes], [have_perl_expect=no])
AS_IF([test "x$have_perl_expect" = "xno"],[
    AC_MSG_WARN([No perl module Expect, will not run automatic test for CLI])
])
AM_CONDITIONAL([HAVE_PERL_EXPECT],[test "x$have_perl_expect" = "xyes"])


dnl --------------------------------------------------------------------------
dnl Documentation support

AC_ARG_ENABLE(doc,
	AS_HELP_STRING([--enable-doc=yes], [Build man pages (yes by default)]),
	[wantdoc=$enableval], [wantdoc=yes])

AC_MSG_CHECKING([for building documentation])
AS_IF([test "x$wantdoc" = "xyes"], [
	AC_MSG_RESULT([yes])
	AC_CHECK_PROGS([ASCIIDOC], [asciidoc])
	AS_IF([test $ASCIIDOC], [
	], [
		AC_MSG_ERROR([asciidoc is required to build man pages])
	])],
[
	AC_MSG_RESULT([no])
])
AM_CONDITIONAL(WANT_DOC, [test "x$wantdoc" = "xyes"])

dnl --------------------------------------------------------------------------

AC_OUTPUT([Makefile
           src/Makefile
           test/Makefile
           doc/Makefile])
