
noinst_LIBRARIES = libgpwsafe.a

libgpwsafe_a_SOURCES = \
		Defs.hh \
		Field.cc Field.hh \
		File3.cc File3.hh \
		Database.cc Database.hh \
		Entry.cc Entry.hh \
		Sha256.cc Sha256.hh \
		Twofish.cc Twofish.hh \
		Hmac.cc Hmac.hh \
		KeyStretch.cc KeyStretch.hh \
		Memory.cc Memory.hh \
		Terminal.cc Terminal.hh \
		Debug.cc Debug.hh

bin_PROGRAMS = gpwsafe

gpwsafe_SOURCES = main.cc \
		  App.cc App.hh

gpwsafe_LDADD = libgpwsafe.a $(LIBGCRYPT_LIBS)

AM_CXXFLAGS = -Wall \
	      $(LIBGCRYPT_CFLAGS) -DGCRYPT_NO_DEPRECATED \
	      $(BOOST_CPPFLAGS)
