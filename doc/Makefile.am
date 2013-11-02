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


EXTRA_DIST = gpwsafe.1.txt formatV3.txt

if WANT_DOC

ASCIIDOC_OPTS = \
	-a version="$(PACKAGE_VERSION)" \
	-a date="`date +%Y-%m-%d`"
if ENABLE_XCLIP
ASCIIDOC_OPTS += -a enable_xclip="yes"
endif #ENABLE_XCLIP

A2X_OPTS = \
	-d manpage \
	-f manpage \
	--destination-dir='./'

man_MANS = gpwsafe.1

gpwsafe.1: gpwsafe.1.txt Makefile.am
	$(AM_V_GEN)a2x $(A2X_OPTS) \
		--asciidoc-opts="$(ASCIIDOC_OPTS) --out-file=./$@.xml" \
		$(srcdir)/$@.txt

# Files that should be removed
MOSTLYCLEANFILES = gpwsafe.1

endif # WANT_DOC
