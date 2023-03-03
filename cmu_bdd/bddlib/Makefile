DESTBIN = $(CMUBDDHOME)/bin
DESTINCLUDE = $(CMUBDDHOME)/include
DESTLIB = $(CMUBDDHOME)/lib/$(HOSTTYPE)
DESTMAN = $(CMUBDDHOME)/man


binaries = 

archives = $(bddlibrary)

includes = $(bddincludes)

manpages = $(bddmanpages)


bddlibrary = libbdd.a

bddincludes = bdduser.h bddint.h

bddmanpages = bdd.3


otherincludes = -I$(DESTINCLUDE)

INCLUDEDIRS = -I. $(otherincludes)

GCC = gcc

gccwarnings = -Wall -Wpointer-arith -Wconversion -Wstrict-prototypes -Wmissing-prototypes

GCCFLAGS = -O2 -g $(gccwarnings) $(INCLUDEDIRS)

CFLAGS = -g $(INCLUDEDIRS)


sources = \
  bdd.c \
  bddapply.c \
  bddassoc.c \
  bddblk.c \
  bddcache.c \
  bddcmp.c \
  bddcomp.c \
  bdddump.c \
  bddhash.c \
  bddmisc.c \
  bddprimes.c \
  bddprint.c \
  bddprprofile.c \
  bddqnt.c \
  bddreduce.c \
  bddrelprod.c \
  bddreorder.c \
  bddsat.c \
  bddsize.c \
  bddsupport.c \
  bddswap.c \
  bddunique.c \
  bddwarn.c \
  mtbdd.c

objects = \
  bdd.o \
  bddapply.o \
  bddassoc.o \
  bddblk.o \
  bddcache.o \
  bddcmp.o \
  bddcomp.o \
  bdddump.o \
  bddhash.o \
  bddmisc.o \
  bddprimes.o \
  bddprint.o \
  bddprprofile.o \
  bddqnt.o \
  bddreduce.o \
  bddrelprod.o \
  bddreorder.o \
  bddsat.o \
  bddsize.o \
  bddsupport.o \
  bddswap.o \
  bddunique.o \
  bddwarn.o \
  mtbdd.o


all: install testbdd


install: $(archives) $(includes) $(manpages)
	for archive in $(archives); do \
	  mv $$archive $(DESTLIB); \
	done
	for include in $(includes); do \
	  cp -f $$include $(DESTINCLUDE); \
	done
	for manpage in $(manpages); do \
	  case $$manpage in \
	    *.1 ) cp -f $$manpage $(DESTMAN)/man1;; \
	    *.3 ) cp -f $$manpage $(DESTMAN)/man3;; \
	  esac; \
	done


$(bddlibrary): $(objects)
	ar r $@ $(objects)
	-ranlib $@


testbdd: testbdd.o
	$(CC) -o $@ testbdd.o -L. -L$(DESTLIB) -lbdd -lmem


dependencies: $(bddincludes) $(sources) testbdd.c
	$(CC) -MM $(INCLUDEDIRS) $(sources) testbdd.c > $@


dist.tar: README dependencies Makefile $(sources) testbdd.c $(bddincludes) $(bddmanpages)
	tar cvf $@ README dependencies Makefile $(sources) testbdd.c $(bddincludes) $(bddmanpages)


makeobj: $(objects)


buildiscas: buildiscas.o $(bddlibrary)
	$(CC) -o $@ buildiscas.o $(bddlibrary) -L$(DESTLIB) -lmem


include dependencies
