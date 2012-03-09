
AM_CXXFLAGS = -Wall \
	      $(LIBGCRYPT_CFLAGS) -DGCRYPT_NO_DEPRECATED \
	      $(BOOST_CPPFLAGS)

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
		Random.cc Random.hh \
		Memory.cc Memory.hh \
		Gcrypt.cc Gcrypt.hh \
		Debug.cc Debug.hh

bin_PROGRAMS = gpwsafe

gpwsafe_SOURCES = main.cc \
		  App.cc App.hh \
		  IEmitter.hh \
		  Terminal.cc Terminal.hh \
		  StdoutEmitter.cc StdoutEmitter.hh

gpwsafe_LDADD = libgpwsafe.a \
		$(LIBGCRYPT_LIBS)

gpwsafe_CXXFLAGS = -Werror $(AM_CXXFLAGS)

if ENABLE_GTK
gpwsafe_SOURCES += GtkEmitter.cc GtkEmitter.hh
gpwsafe_LDADD += $(GTK_LIBS)
gpwsafe_CXXFLAGS += $(GTK_CFLAGS)
endif # Enable_GTK

