
TESTS = KeyStretchTest \
	File3Test \
	EntryTest

check_PROGRAMS = KeyStretchTest \
		 File3Test \
		 EntryTest

KeyStretchTest_SOURCES = KeyStretchTest.cc
KeyStretchTest_LDADD = $(top_builddir)/src/libgpwsafe.a \
		       $(LIBGCRYPT_LIBS) \
		       $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

File3Test_SOURCES = File3Test.cc
File3Test_LDADD = $(top_builddir)/src/libgpwsafe.a \
		  $(LIBGCRYPT_LIBS) \
		  $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

EntryTest_SOURCES = EntryTest.cc
EntryTest_LDADD = $(top_builddir)/src/libgpwsafe.a \
		  $(LIBGCRYPT_LIBS) \
		  $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

AM_CPPFLAGS = $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK
