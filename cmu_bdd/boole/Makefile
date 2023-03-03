DESTBIN = $(CMUBDDHOME)/bin
DESTINCLUDE = $(CMUBDDHOME)/include
DESTLIB = $(CMUBDDHOME)/lib/$(HOSTTYPE)
DESTMAN = $(CMUBDDHOME)/man


binaries = boole

archives = 

includes = 

manpages = 


otherincludes = -I$(DESTINCLUDE)

INCLUDEDIRS = -I. $(otherincludes)

LIBDIRS = -L$(DESTLIB)

GCC = gcc

gccwarnings = -Wall -Wpointer-arith -Wconversion -Wstrict-prototypes -Wmissing-prototypes

GCCFLAGS = -O2 -g $(gccwarnings) $(INCLUDEDIRS)

CFLAGS = -g $(INCLUDEDIRS)


sources = \
  grammar.c \
  hash.c \
  input.c \
  main.c \
  vec.c

objects = \
  grammar.o \
  hash.o \
  input.o \
  main.o \
  vec.o


all: install


install: $(binaries) $(archives) $(includes) $(manpages)
	cp -f $(binaries) $(DESTBIN)


boole: $(objects)
	$(CC) -o $@ $(objects) $(LIBDIRS) -lbdd -lmem


grammar.c y.tab.h: grammar.y
	$(YACC) -d grammar.y
	mv y.tab.c $@


input.c: input.l
	$(LEX) -t input.l > $@


dependencies: $(sources)
	$(CC) -MM $(INCLUDEDIRS) $(sources) > $@


dist.tar: README dependencies Makefile $(sources) input.l grammar.y boole.h hash.h vec.h y.tab.h
	tar cvf $@ README dependencies Makefile $(sources) input.l grammar.y boole.h hash.h vec.h y.tab.h


makeobj: $(objects)


include dependencies
