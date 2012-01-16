
TESTS = file3-test KeyStretchTest

check_PROGRAMS = file3-test KeyStretchTest

file3_test_SOURCES = file3.cc

KeyStretchTest_SOURCES = KeyStretchTest.cc
KeyStretchTest_LDADD = $(top_builddir)/src/libgpwsafe.a \
		       $(LIBGCRYPT_LIBS) \
		       $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

AM_CPPFLAGS = $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK
