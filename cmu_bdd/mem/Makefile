DESTBIN = $(CMUBDDHOME)/bin
DESTINCLUDE = $(CMUBDDHOME)/include
DESTLIB = $(CMUBDDHOME)/lib/$(HOSTTYPE)
DESTMAN = $(CMUBDDHOME)/man


binaries = 

archives = $(memlibrary)

includes = $(memincludes)

manpages = $(memmanpages)


memlibrary = libmem.a

memincludes = memuser.h

memmanpages = mem.3


otherincludes = -I$(CMUBDDHOME)/include

INCLUDEDIRS = -I. $(otherincludes)

GCC = gcc

gccwarnings = -Wall -Wpointer-arith -Wconversion -Wstrict-prototypes -Wmissing-prototypes -Wtraditional

GCCFLAGS = -O2 -g $(gccwarnings) $(INCLUDEDIRS)

CFLAGS = -g $(INCLUDEDIRS)


sources = \
  memblock.c \
  memrec.c

objects = \
  memblock.o \
  memrec.o


all: install


install: $(archives) $(includes) $(manpages)
	for archive in $(archives); do \
	  mv $$archive $(DESTLIB); \
	done
	for include in $(includes); do \
	  cp -f $$include $(DESTINCLUDE); \
	done
	for manpage in $(manpages); do \
	  case $$manpage in \
	    *.0 ) cp -f $$manpage $(DESTMAN)/man0;; \
	    *.1 ) cp -f $$manpage $(DESTMAN)/man1;; \
	    *.2 ) cp -f $$manpage $(DESTMAN)/man2;; \
	    *.3 ) cp -f $$manpage $(DESTMAN)/man3;; \
	    *.4 ) cp -f $$manpage $(DESTMAN)/man4;; \
	    *.5 ) cp -f $$manpage $(DESTMAN)/man5;; \
	    *.6 ) cp -f $$manpage $(DESTMAN)/man6;; \
	    *.7 ) cp -f $$manpage $(DESTMAN)/man7;; \
	    *.8 ) cp -f $$manpage $(DESTMAN)/man8;; \
	  esac; \
	done


$(memlibrary): $(objects)
	ar r $@ $(objects)
	-ranlib $@


dependencies: $(bddincludes) $(sources)
	$(CC) -MM $(INCLUDEDIRS) $(sources) > $@


dist.tar: README dependencies Makefile $(sources) $(memincludes) memint.h $(memmanpages)
	tar cvf $@ README dependencies Makefile $(sources) $(memincludes) memint.h $(memmanpages)


makeobj: $(objects)


include dependencies
