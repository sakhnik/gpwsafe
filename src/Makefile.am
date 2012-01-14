
bin_PROGRAMS = gpwsafe

gpwsafe_SOURCES = main.cc Defs.hh \
		  File3.cc File3.hh \
		  Sha256.cc Sha256.hh \
		  KeyStretch.cc KeyStretch.hh \
		  Debug.cc Debug.hh

AM_CXXFLAGS = -Wall $(LIBGCRYPT_CFLAGS) -DGCRYPT_NO_DEPRECATED
LDADD = $(LIBGCRYPT_LIBS)
