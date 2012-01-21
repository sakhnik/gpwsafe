
noinst_LIBRARIES = libgpwsafe.a

libgpwsafe_a_SOURCES = \
		Defs.hh \
		Field.cc Field.hh \
		File3.cc File3.hh \
		Sha256.cc Sha256.hh \
		Twofish.cc Twofish.hh \
		Hmac.cc Hmac.hh \
		KeyStretch.cc KeyStretch.hh \
		Memory.cc Memory.hh \
		Debug.cc Debug.hh

bin_PROGRAMS = gpwsafe

gpwsafe_SOURCES = main.cc

gpwsafe_LDADD = libgpwsafe.a $(LIBGCRYPT_LIBS)

AM_CXXFLAGS = -Wall \
	      $(LIBGCRYPT_CFLAGS) -DGCRYPT_NO_DEPRECATED \
	      $(BOOST_CPPFLAGS)
