#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <clock.h>
#include <idtLoader.h>
#include <time.h>
#include <interrupts.h>
#include <defs.h>
#include <irqDispatcher.h>
#include <keyboard.h>
#include <cursor.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

void sysCallWrite(uint64_t fd, char * buffer, uint64_t len);

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrintAtt("Arquitectura de las Computadoras", 2, 15);
	ncNewline();

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{	
	char buffer[9];

	ncPrint("[Kernel Main]");
	ncNewline();

	ncPrint("Time is: ");
	timeToStr(buffer);
	ncPrint(buffer);
	ncNewline();

	ncPrint("Date is: ");
	dateToStr(buffer);
	ncPrint(buffer);
	ncNewline();

	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	load_idt();
	uint8_t printed = 0;
	/*
	while (1){
		if (!printed && ticks_elapsed() % (18*5) == 0){
			printed = 1;
			timeToStr(buffer);
			ncPrint(buffer);
			ncNewline();
		}
		if (printed && ticks_elapsed() % 18 != 0)
			printed = 0;
	}
	*/

	
	while(1){
		char ret = keyboard_handler();
		ncPrintChar(ret);
	}
	

	//sysCallWrite(2, "hola", 5);


	ncPrint("[Finished]");
	return 0;
}
