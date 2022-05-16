include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_IPC=$(wildcard ipc/*.c)
SOURCES_MEM=$(wildcard memManagers/*.c)
SOURCES_PREB=$(wildcard prebuilt/*.c)
SOURCES_SYNC=$(wildcard sync/*.c)
SOURCES_MISC=$(wildcard misc/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_IPC=$(SOURCES_IPC:.c=.o)
OBJECTS_MEM=$(SOURCES_MEM:.c=.o)
OBJECTS_PREB=$(SOURCES_PREB:.c=.o)
OBJECTS_SYNC=$(SOURCES_SYNC:.c=.o)
OBJECTS_MISC=$(SOURCES_MISC:.c=.o)



LOADERSRC=loader.asm

GCCDEFS=

LOADEROBJECT=$(LOADERSRC:.asm=.o)

ALL_OBJECTS_KERNEL = $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_IPC) $(OBJECTS_MEM) $(OBJECTS_PREB) $(OBJECTS_SYNC) $(OBJECTS_MISC)

all: $(KERNEL)

buddy: GCCDEFS=-DBUDDY
buddy: $(KERNEL)

$(KERNEL): $(STATICLIBS) $(ALL_OBJECTS_KERNEL)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(ALL_OBJECTS_KERNEL) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) $(GCCDEFS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin
	cd drivers; rm -rf *.o
	cd ipc; rm -rf *.o
	cd memManagers; rm -rf *.o
	cd prebuilt; rm -rf *.o
	cd sync; rm -rf *.o
	cd misc; rm -rf *.o
	
.PHONY: all clean
