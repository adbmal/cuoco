
v=debug

########## options ##########
#CFLAGS += -g -fno-strict-aliasing -O2 -Wall -export-dynamic \
		-Wall -pipe  -D_GNU_SOURCE -D_REENTRANT -fPIC -Wno-deprecated
CFLAGS += -g -O0 -Wall -pipe -fPIC

#LINKS += -g -L./lib -lcolib -lpthread -ldl
LINKS += -g

ifneq ($v,release)
BFLAGS= -g
endif


##define the compliers
CPP = clang++
CC  = clang
AR = ar -rc
RANLIB = ranlib

BUILDEXE = $(CPP) $(BFLAGS) -o $@ $^ $(LINKS)
CLEAN = rm -f *.o


CPPCOMPILE = $(CPPCOMPI) $< $(FLAGS) $(INCLS) $(MTOOL_INCL) -o $@
CCCOMPILE = $(CCCOMPI) $< $(FLAGS) $(INCLS) $(MTOOL_INCL) -o $@
CCCOMPI=$(CC) $(CFLAGS)

.c.o:
	$(CCCOMPILE)

all:
	clang -o cuoco -g -fno-inline-small-functions -Wall cuoco.c cuoco_ctx_swap.S

cuoco: cuoco.o
	$(BUILDEXE)
