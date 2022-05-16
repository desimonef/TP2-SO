include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin

SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_TESTS=$(wildcard tests/*.c)
SOURCES_LIBRARIES=$(wildcard libraries/*.c)
SOURCES_MISC=$(wildcard misc/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_TESTS=$(SOURCES_TESTS:.c=.o)
OBJECTS_LIBRARIES=$(SOURCES_LIBRARIES:.c=.o)
OBJECTS_MISC=$(SOURCES_MISC:.c=.o)

ALL_OBJECTS = $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_LIBRARIES) $(OBJECTS_MISC) $(OBJECTS_TESTS)

all: $(MODULE)

$(MODULE): $(SOURCES) $(ALL_OBJECTS) _loader.o
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld _loader.o $(ALL_OBJECTS) -o ../$(MODULE)

_loader.o: _loader.c 
	$(GCC) $(GCCFLAGS) -c _loader.c

%.o: %.c 
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@


clean:
	rm -rf asm/*.o *.o 
	cd tests; rm -rf *.o
	cd libraries; rm -rf *.o
	cd misc; rm -rf *.o
.PHONY: all clean print
