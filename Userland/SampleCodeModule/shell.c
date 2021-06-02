
#include "stdlib.h"
#include "userlib.h"
#include "shell.h"

#define MAXLEN 100

void initShell(){
    clearScreen();
    shellWelcomeMsg();
    shell();
}

void shell() {
    char command[MAXLEN] = {0};
    char arg[MAXLEN] = {0};
    while(1) {
        printf("$ ");
        *command = 0;
        *arg = 0;
        scanf("%s %s", command, arg);
        execute(command, arg); 
    }
}

void execute(const char * command, const char * arg) {
    if(strcmp(command, "toto") == 0)
        printf("toto");
}
/*
void printMem(char * hexaAddress){
	unsigned char buffer[35]; // Aca guardo los valores de los registros
	char byte[10]; // Guardo cada byte individualmente para imprimir
	dumpMem(buffer, hexToInt(hexaAddress), 32);
	printf("Dump de 32 bytes a partir de la direccion %s :\n", hexaAddress);
	for(int i = 0; i < 32; i++){
		if(i%4 == 0){
			printf("\n");
		}
		printf("    %s ", intToHex(buffer[i], byte, 1));
	}
	printf("\n");
}
*/

void shellWelcomeMsg(){
    printf("          -+#%%%%%%*-\n");              
    printf("        =@*-.......-*@=\n");            
    printf("       -@=....-+*#%%*@@#.\n");          
    printf("       @*...:*@+-:....:+*#:\n");        
    printf("   .:-+@+...*@*=:::.   .:+@=\n");       
    printf(" -@#++#@-...+@**+==------+@+\n");       
    printf(" *#:::#@-....*@#********#@#\n");        
    printf(" @#---@*-.....=*%%%%%%#**@-\n");        
    printf(".@*---@#-:..............:@=\n");        
    printf(".@*---@#--..............-@=\n");        
    printf(" @*---@#---.............+@-\n");        
    printf(" @*---@*-----:::.....:--*@.\n");        
    printf(" =@#+=%%----------------#@\n");         
    printf("  .-+**@------#%%*@*#--=@*\n");         
    printf("      -@+----=@+ +@=---*@:\n");         
    printf("       @*----=@+ :@*===@#\n");          
    printf("      +@%##%**:  :+##*+\n");           
    
}