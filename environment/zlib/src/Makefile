# Makefile for zlib
# Copyright (C) 1995-2005 Jean-loup Gailly.
# For conditions of distribution and use, see copyright notice in zlib.h

# To compile and test, type:
#    ./configure; make test
# The call of configure is optional if you don't have special requirements
# If you wish to build zlib as a shared library, use: ./configure -s

# To use the asm code, type:
#    cp contrib/asm?86/match.S ./match.S
#    make LOC=-DASMV OBJA=match.o

# To install /usr/local/lib/libz.* and /usr/local/include/zlib.h, type:
#    make install
# To install in $HOME instead of /usr/local, use:
#    make install prefix=$HOME

CC=gcc

CFLAGS=-O
#CFLAGS=-O -DMAX_WBITS=14 -DMAX_MEM_LEVEL=7
#CFLAGS=-g -DDEBUG
#CFLAGS=-O3 -Wall -Wwrite-strings -Wpointer-arith -Wconversion \
#           -Wstrict-prototypes -Wmissing-prototypes
CFLAGS = -O3 -Wall -Wwrite-strings -Wpointer-arith -Wconversion \
            -Wstrict-prototypes -Wmissing-prototypes \
            -D__GW32__ -D_LARGEFILE_SOURCE=1 -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 \
            -mms-bitfields  -fno-exceptions -fomit-frame-pointer -march=i386 -mcpu=i686  \
   -idirafter e:/progra~1/gnuwin32/include -idirafter h:/glibc/include
 
LDFLAGS=-Wl,-s $(VERSADD)
LDSHARED=o2dll
CPP=$(CC) -E

MAJOR=1
MINOR=2
BUILD=3
VERSION = $(MAJOR).$(MINOR).$(BUILD)
CURRENT = 4# MAJOR + BUILD
REVISION = $(MINOR)
AGE = $(BUILD)
LTVERSION = $(CURRENT):$(REVISION):$(AGE)
VERSADD = -Wl,--major-image-version=$(MAJOR) -Wl,--minor-image-version=$(MINOR) 

LIBS=libz.a
SHAREDLIB=libz.so
SHAREDLIBV=libz.so.1.2.3
SHAREDLIBM=libz.so.1
STATLIB=libz.a
IMPLIB=libz.dll.a
LIBS=$(STATLIB) $(IMPLIB) zlib.lib zlib-bcc.lib
SHAREDLIB=zlib.dll
SHAREDLIBV=zlib1.dll
SHAREDLIBM=zlib121.dll
EXTRALIBS=-Le:/progra~1/gnuwin32/lib -lgw32c

AR=ar rc
RANLIB=ranlib
TAR=tar
SHELL=/bin/sh
EXE=.exe

.SUFFIXES:
.SUFFIXES: .c .rc .o .exe

%.E : %.c
	$(CPP) $(CFLAGS) -o $@ $<
%.pic.o : CFLAGS += -DZLIB_DLL
%.pic.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<
# Rule to make compiled resource (Windows)
.rc.o:
	windres -i $< -o $@

prefix = c:/progra~1/Zlib
exec_prefix = ${prefix}
bindir = ${exec_prefix}/lib
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include
mandir = ${prefix}/man
man3dir = ${mandir}/man3

OBJS = adler32.o compress.o crc32.o gzio.o uncompr.o deflate.o trees.o \
       zutil.o inflate.o infback.o inftrees.o inffast.o
DLLOBJS = $(OBJS:.o=.pic.o) zlib-dll-res.o zlib-dllversion.o

OBJA =
# to use the asm code: make OBJA=match.o

TEST_OBJS = example.o minigzip.o

all: example minigzip example-static minigzip-static

check: test
test: all
	@LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH) ; export LD_LIBRARY_PATH; \
	echo hello world | ./minigzip | ./minigzip -d || \
	  echo '		*** minigzip test FAILED ***' ; \
	if ./example; then \
	  echo '		*** zlib test OK ***'; \
	else \
	  echo '		*** zlib test FAILED ***'; \
	fi

$(IMPLIB): $(STATLIB) $(SHAREDLIBV)

$(STATLIB): $(OBJS) $(OBJA)
	$(AR) $@ $(OBJS) $(OBJA)
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1

match.o: match.S
	$(CPP) match.S > _match.s
	$(CC) -c _match.s
	mv _match.o match.o
	rm -f _match.s

$(SHAREDLIBV): $(DLLOBJS)
	echo $(LDSHARED) -o $(SHAREDLIB) -l $(IMPLIB) $(DLLOBJS) $(LDFLAGS) -version-info $(LTVERSION) $(EXTRALIBS)
	$(LDSHARED) -o $(SHAREDLIB) -l $(IMPLIB) $(DLLOBJS) $(LDFLAGS) -version-info $(LTVERSION) $(EXTRALIBS)
	rm -f $(SHAREDLIB) $(SHAREDLIBM)
#	ln -s $@ $(SHAREDLIB)
#	ln -s $@ $(SHAREDLIBM)

example-static: example.o example-res.o $(STATLIB)
	$(CC) $(CFLAGS) -o $@ example.o  example-res.o $(LDFLAGS) libz.a $(EXTRALIBS)

example: example.pic.o example-res.o $(IMPLIB)
	$(CC) $(CFLAGS) -o $@ example.pic.o  example-res.o $(LDFLAGS) libz.dll.a $(EXTRALIBS)

minigzip-static: minigzip.o minigzip-res.o $(STATLIB)
	$(CC) $(CFLAGS) -o $@ minigzip.o  minigzip-res.o $(LDFLAGS) libz.a $(EXTRALIBS)

minigzip: minigzip.pic.o minigzip-res.o $(IMPLIB)
	$(CC) $(CFLAGS) -o $@ minigzip.pic.o  minigzip-res.o $(LDFLAGS) libz.dll.a $(EXTRALIBS)

install: $(LIBS)
	-@if [ ! -d $(exec_prefix) ]; then mkdir -p $(exec_prefix); fi
	-@if [ ! -d $(includedir)  ]; then mkdir -p $(includedir); fi
	-@if [ ! -d $(libdir)      ]; then mkdir -p $(libdir); fi
	-@if [ ! -d $(bindir)      ]; then mkdir -p $(bindir); fi
	-@if [ ! -d $(man3dir)     ]; then mkdir -p $(man3dir); fi
	cp -fp zlib.h zconf.h $(includedir)
	chmod 644 $(includedir)/zlib.h $(includedir)/zconf.h
	cp -fp $(LIBS) $(libdir)
	cd $(libdir); chmod 755 $(LIBS)
	-@(cd $(libdir); $(RANLIB) libz.a || true) >/dev/null 2>&1
	cp -fp $(SHAREDLIBV) $(bindir)
	cd $(bindir); chmod 755 $(SHAREDLIBV); \
#	if test -f $(SHAREDLIBV); then \
	  rm -f $(SHAREDLIB) $(SHAREDLIBM); \
	  ln -s $(SHAREDLIBV) $(SHAREDLIB); \
	  ln -s $(SHAREDLIBV) $(SHAREDLIBM); \
	  (ldconfig || true)  >/dev/null 2>&1; \
	fi
	cp -fp zlib.3 $(man3dir)
	chmod 644 $(man3dir)/zlib.3
# The ranlib in install is needed on NeXTSTEP which checks file times
# ldconfig is for Linux

uninstall:
	cd $(includedir); \
	cd $(libdir); rm -f libz.a; \
	if test -f $(SHAREDLIBV); then \
	  rm -f $(SHAREDLIBV) $(SHAREDLIB) $(SHAREDLIBM); \
	fi
	cd $(man3dir); rm -f zlib.3

mostlyclean: clean
clean:
	rm -f *.o *.pic.o *~ example$(EXE) minigzip$(EXE) \
	   libz.* $(LIBS) $(SHAREDLIBV) foo.gz so_locations \
	   _match.s maketree contrib/infback9/*.o

maintainer-clean: distclean
distclean: clean
	cp -p Makefile.in Makefile
	cp -p zconf.in.h zconf.h
	rm -f .DS_Store

tags:
	etags *.[ch]

depend:
	makedepend -- $(CFLAGS) -- *.[ch]

# DO NOT DELETE THIS LINE -- make depend depends on it.

adler32.o: zlib.h zconf.h
compress.o: zlib.h zconf.h
crc32.o: crc32.h zlib.h zconf.h
deflate.o: deflate.h zutil.h zlib.h zconf.h
example.o: zlib.h zconf.h
gzio.o: zutil.h zlib.h zconf.h
inffast.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
inflate.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
infback.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
inftrees.o: zutil.h zlib.h zconf.h inftrees.h
minigzip.o: zlib.h zconf.h
trees.o: deflate.h zutil.h zlib.h zconf.h trees.h
uncompr.o: zlib.h zconf.h
zutil.o: zutil.h zlib.h zconf.h
