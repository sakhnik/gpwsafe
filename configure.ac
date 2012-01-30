AC_INIT([gpwsafe],[0.1.2],[sakhnik@gmail.com])
AC_PREREQ(2.59)
dnl AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([1.9 color-tests])

AM_CONFIG_HEADER([config.h])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

AC_CHECK_HEADERS(uuid/uuid.h)
AC_CHECK_FUNCS(uuid_generate, uuid)
AC_SEARCH_LIBS(uuid_generate, uuid)

AC_CHECK_FUNCS(getopt_long)
AC_CHECK_LIB(readline,readline,[],[AC_MSG_ERROR(cannot find libreadline)])

AM_PATH_LIBGCRYPT([1.2.0],[],[])

m4_include([m4/ax_boost_base.m4])
AX_BOOST_BASE([1.40],[],[])
m4_include([m4/ax_boost_unit_test_framework.m4])
AX_BOOST_UNIT_TEST_FRAMEWORK

dnl --------------------------------------------------------------------------
dnl X clipboard support
AC_ARG_ENABLE(
    [xclip],
    AS_HELP_STRING([--enable-xclip=[yes]],
                   [Enable copying to the X clipboard (yes,no,gtk)])
)

AC_MSG_CHECKING(--enable-xclip argument)
AS_CASE([$enable_xclip],
    [no],    [AC_MSG_RESULT(no X clipboard support)],
    [yes|""],[AC_MSG_RESULT(yes - automatic X clipboard support)
              enable_xclip=yes],
    [AC_MSG_RESULT([Sorry, $enable_xclip X clipboard is not supported])]
)

AS_IF([test "x$enable_xclip" = "xgtk" -o "x$enable_xclip" = "xyes"],[
    PKG_CHECK_MODULES([GTK],[gtk+-3.0],[gtk_found=yes enable_xclip=gtk])
    AS_IF([test "x$gtk_found" != "xyes"],[
        PKG_CHECK_MODULES([GTK],[gtk+-2.0],[gtk_found=yes enable_xclip=gtk])
    ])
])
AS_IF([test "x$gtk_found" = "xyes"],[
    AC_DEFINE([ENABLE_GTK],[1],[Define to compile with GTK+ support])
    AC_DEFINE([ENABLE_XCLIP],[1],[Define to compile with X clipboard support])
])
AM_CONDITIONAL([ENABLE_GTK],[test "x$gtk_found" = "xyes"])

AS_IF([test "x$enable_xclip" = "xyes"],[
    AC_MSG_WARN([No suitable X clipboard support library found])
    enable_xclip=no
])
AM_CONDITIONAL([ENABLE_XCLIP],[test "x$enable_xclip" != "xno"])

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
