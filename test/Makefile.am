
TESTS = KeyStretchTest

check_PROGRAMS = KeyStretchTest

KeyStretchTest_SOURCES = KeyStretchTest.cc
KeyStretchTest_LDADD = $(top_builddir)/src/libgpwsafe.a \
		       $(LIBGCRYPT_LIBS) \
		       $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

AM_CPPFLAGS = $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK
