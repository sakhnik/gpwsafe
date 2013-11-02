# This file is part of gpwsafe.
#
# Gpwsafe is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Gpwsafe is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gpwsafe.  If not, see <http://www.gnu.org/licenses/>


AM_CXXFLAGS = -Wall \
		$(LIBGCRYPT_CFLAGS) -DGCRYPT_NO_DEPRECATED \
		$(BOOST_CPPFLAGS)

noinst_LIBRARIES = libgpwsafe.a

libgpwsafe_a_SOURCES = \
		Database.cc Database.hh \
		Debug.cc Debug.hh \
		Defs.hh \
		Entry.cc Entry.hh \
		Field.cc Field.hh \
		File3.cc File3.hh \
		Gcrypt.cc Gcrypt.hh \
		Hmac.cc Hmac.hh \
		KeyStretch.cc KeyStretch.hh \
		Memory.cc Memory.hh \
		Random.cc Random.hh \
		Sha256.cc Sha256.hh \
		Twofish.cc Twofish.hh

bin_PROGRAMS = gpwsafe

gpwsafe_SOURCES = \
		main.cc \
		App.cc App.hh \
		Command.cc Command.hh \
		CommandAdd.cc CommandAdd.hh \
		CommandCreate.cc CommandCreate.hh \
		CommandEdit.cc CommandEdit.hh \
		CommandList.cc CommandList.hh \
		Exceptions.hh \
		IEmitter.hh \
		StdoutEmitter.cc StdoutEmitter.hh \
		Terminal.cc Terminal.hh

gpwsafe_LDADD = libgpwsafe.a \
		$(LIBGCRYPT_LIBS) \
		$(BOOST_PROGRAM_OPTIONS_LIB)

gpwsafe_CXXFLAGS = -Werror $(AM_CXXFLAGS)

if ENABLE_GTK
gpwsafe_SOURCES += GtkEmitter.cc GtkEmitter.hh
gpwsafe_LDADD += $(GTK_LIBS)
gpwsafe_CXXFLAGS += $(GTK_CFLAGS)
endif # Enable_GTK
