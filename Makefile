# file: Makefile
# vim:fileencoding=utf-8:fdm=marker:ft=make
#
# Author: R.F. Smith <rsmith@xs4all.nl>
# Copyright © 2023 R.F. Smith <rsmith@xs4all.nl>
# Created: 2023-05-05T11:30:55+0200
# Last modified: 2023-05-05T11:36:05+0200

# Compilation flags for optimized libraries.
#CFLAGS = -pipe -O2 -Wall -Wstrict-prototypes -fomit-frame-pointer -fPIC

# The next line is for building debugging libraries.
CFLAGS = -pipe -g -Wall -Wstrict-prototypes -fPIC

# Extra libraries to be linked.
LIBS = 

# Where to search for additional header files.
#HDIRS = -I/usr/X11R6/include

# Where to install the library and header file.
# THESE SHOULD BE CHECKED AND CHANGED IF NECESSARY!
LDEST = /usr/local/lib
HDEST = /usr/local/include

# 'lib' wil be prepended automagically to the basename.
# for libfoo.so.0.1.2 the basename is foo, version_major is 0,
# version_minor is 1 and patchlevel is 2.
BASENAME      = arena
VERSION_MAJOR = 1
VERSION_MINOR = 0
PATCHLEVEL    = 0

# Name of the library header file.
HDRS = arena.h

# list of source files
SRC = arena.c

# Regular files to include in the distribution
DISTFILES = Makefile

# Extra stuff to add into the distribution.
XTRA_DIST = 

##### No editing necessary beyond this point ####
.SUFFIXES:
.SUFFIXES: .c .o

.PHONY:	clean help all shared static install_shared install_static \
install uninstall dist backup is_root check_dir

NUM2      = $(VERSION_MAJOR).$(VERSION_MINOR)
NUM3      = $(VERSION_MAJOR).$(VERSION_MINOR).$(PATCHLEVEL)
SHARED	 = lib$(BASENAME).so.$(NUM3)
STATIC	 = lib$(BASENAME).a
PKGDIR    = lib$(BASENAME)-$(NUM3)
TARFILE	 = lib$(BASENAME)-$(NUM3).tar.gz
BACKUP	 = lib$(BASENAME)-backup-$(NUM3).tar.gz
OBJS		 = $(SRC:.c=.o)

INSTALL   = install

# This is the default target.
help:;	@echo "Use one of the following invocations:";\
		echo "  \"make shared\"         to build the shared library";\
		echo "  \"make static\"         to build a static library";\
		echo "  \"make all\"            to build both libraries";\
#		echo "  \"make test\"           to build test programs";\
		echo "  \"make depend\"         to update the dependancies";\
		echo "  \"make clean\"          to remove all generated files";\
		echo "  \"make ps\"             to build man-pages in PostScript";\
		echo "  \"make txt\"            to build text man-pages";\
		echo "  \"make html\"           to build man-pages in html";\
		echo "NOTE: you must be root to use the following targets:";\
		echo "  \"make install_shared\" to install the shared library";\
		echo "  \"make install_static\" to install the static library";\
		echo "  \"make install_man\"    to install the manual-page";\
		echo "  \"make install\"        to install libraries and man-pages";\
		echo "  \"make uninstall\"      to remove the installed libraries";\
		echo "";\
		echo "The usual invocation would be to do \"make all\",";\
		echo "\"make test\", login as root, and then do a \"make install\".";\
		echo ""

shared:		$(SHARED)
static:		$(STATIC)
all:		$(SHARED) $(STATIC)

# Build the shared library.
$(SHARED):	$(OBJS)
	$(CC) -shared -Wl,-soname,lib$(BASENAME).so.$(VERSION_MAJOR) \
	-o $(SHARED) $(OBJS) $(LIBS)
#	strip -g --strip-unneeded $(SHARED)

# Builds a static- (archive) library instead of a shared library.
$(STATIC): $(OBJS)
	ar crus $(STATIC) $(OBJS)
#	strip -g $(STATIC)

# Remove all files that can be created by this makefile.
clean:;
	-rm -f $(OBJS) core *~ $(SHARED) $(STATIC) $(TARFILE) \

# Check if the user has root privileges.
is_root:;
	@if [ `id -u` != 0 ]; then \
		echo "You must be root to install the library!"; \
		exit 1; \
	fi

# Check if the install directory isn't the current directory.
check_dir:;
	@if [ $(LDEST) = . -o $(HDEST) = . ]; then \
		echo "Can't install in current directory!";\
		exit 1; \
	fi

# Install the header files
install_hdr: $(HDRS)
	list='$(HDRS)'; \
	for i in $$list; do \
		rm -f $(MDEST)/$$i; \
		$(INSTALL) -m 644 $$i $(MDEST); \
	done

install_man: $(MPAGES)
	list='$(MPAGES)'; \
	for i in $$list; do \
		rm -f $(MDEST)/$$i; \
		$(INSTALL) -m 644 $$i $(MDEST); \
	done

# Install the shared library. Must be run as root.
install_shared: $(SHARED) is_root check_dir install_hdr install_man
   # Remove the old library.
	rm -f $(LDEST)/$(SHARED)
	rm -f $(LDEST)/lib$(BASENAME).so.$(VERSION_MAJOR)
	rm -f $(LDEST)/lib$(BASENAME).so
   # Install new library and make the usual links.
	$(INSTALL) -m 755 $(SHARED) $(LDEST) ; cd $(LDEST) ;\
	ln -s $(SHARED) lib$(BASENAME).so.$(VERSION_MAJOR) ;\
	ln -s $(SHARED) lib$(BASENAME).so 

# Install the static library. Must be run as root.
install_static: $(STATIC) is_root check_dir install_hdr install_man
   # Remove the old library.
	rm -f $(LDEST)/$(STATIC)
   # Install new library.
	$(INSTALL) -m 644 $(STATIC) $(LDEST)

# Installs both libraries
install: install_shared install_static

# Remove the installed libraries, header and man-page. Must be run as root.
uninstall: is_root
	rm -f $(LDEST)/$(SHARED)
	rm -f $(LDEST)/$(STATIC)
	rm -f $(LDEST)/lib$(BASENAME).so.$(VERSION_MAJOR)
	rm -f $(LDEST)/lib$(BASENAME).so
	list='$(HDRS)'; \
	for i in $$list; do \
		rm -f $(HDEST)/$$i; \
	done
	list='$(MPAGES)'; \
	for i in $$list; do \
		rm -f $(MDEST)/$$i; \
	done

# Make a source distribution file.
dist:
	rm -rf $(PKGDIR)
	mkdir -p $(PKGDIR)
	cp $(DISTFILES) $(XTRA_DIST) *.c *.h $(PKGDIR)
	tar -czf $(TARFILE) $(PKGDIR)
	rm -rf $(PKGDIR)

# Make a complete backup of the source directory.
backup: clean
	tar -czf $(BACKUP) *

# postscript manual pages
ps: $(MPAGES:.3=.ps)

# text manual pages
txt: $(MPAGES:.3=.txt)

# html manual pages
html: $(MPAGES:.3=.html)

# Implicit rule (is needed because of HDIRS!)
.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(HDIRS) -c -o $@ $<

%.ps: %.3
	groff -Tps -man $< >$@

%.txt: %.3
	groff -Tlatin1 -man $< >$@

%.html: %.3
	groff -Thtml -man $< >$@
