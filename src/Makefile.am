
bin_PROGRAMS = gpwsafe

gpwsafe_SOURCES = main.cc \
		  File3.cc File3.hh

AM_CXXFLAGS = -Wall $(LIBGCRYPT_CFLAGS) -DGCRYPT_NO_DEPRECATED
LDADD = $(LIBGCRYPT_LIBS)
