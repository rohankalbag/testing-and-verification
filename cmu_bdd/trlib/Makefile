DESTBIN = $(CMUBDDHOME)/bin
DESTINCLUDE = $(CMUBDDHOME)/include
DESTLIB = $(CMUBDDHOME)/lib/$(HOSTTYPE)
DESTMAN = $(CMUBDDHOME)/man

binaries = 

archives = $(trlibrary)

includes = truser.h

manpages = $(trmanpages)


trlibrary = libtr.a

trincludes = truser.h vec.h

trmanpages = tr.3


otherincludes = -I$(DESTINCLUDE) -I$(HOME)/bddlib/

INCLUDEDIRS = -I. $(otherincludes)

GCC = gcc

gccwarnings = -Wall -Wpointer-arith -Wconversion -Wstrict-prototypes -Wmissing-prototypes

GCCFLAGS = -O2 -g $(gccwarnings) $(INCLUDEDIRS)

CFLAGS = -g $(INCLUDEDIRS)


sources = \
  tr.c \
  vec.c

objects = \
  tr.o \
  vec.o


all: install


install: $(binaries) $(archives) $(includes) $(manpages)
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


$(trlibrary): $(objects)
	ar r $@ $(objects)
	-ranlib $@


dependencies: $(trincludes) $(sources)
	$(CC) -MM $(INCLUDEDIRS) $(sources) > $@


dist.tar: README dependencies Makefile $(sources) $(trincludes) $(trmanpages)
	tar cvf $@ README dependencies Makefile $(sources) $(trincludes) $(trmanpages)


makeobj: $(objects)


include dependencies
