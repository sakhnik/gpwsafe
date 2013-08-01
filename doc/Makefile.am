
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
