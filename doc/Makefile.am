
EXTRA_DIST = gpwsafe.1.txt

if WANT_DOC

ASCIIDOC_OPTS = \
	-a version="$(PACKAGE_VERSION)" \
	-a date="`date +%Y-%m-%d`"

A2X_OPTS = \
	-d manpage \
	-f manpage \
	--destination-dir='./'

man_MANS = gpwsafe.1

gpwsafe.1: gpwsafe.1.txt Makefile.am
	$(AM_V_GEN)a2x $(A2X_OPTS) \
	    --asciidoc-opts="$(ASCIIDOC_OPTS) --out-file=./$@.xml" \
	    $(srcdir)/$@.txt

endif # WANT_DOC
