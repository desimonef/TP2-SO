#include "stdlib.h"
#include "userlib.h"
#include "shell.h"
#include "clock.h"

#define MAXLEN 100

void initShell(){
    screenClear();
    //shellWelcomeMsg();
    shell();
    while(1){}
    //shell();
}

void shell(){
    //screenClear();
    char command[MAXLEN] = {0};
    char arg[MAXLEN] = {0};
    int off = 0;
    while(off == 0) {
        printf("\nBienvenidos a nuestro precario sistema operativo\n\nEs el anio %d\n\n", 2021);
        printf("%c : %s\n\n\n", 'a', "TOTO"); //sigue sin reconocer los chars, despues investigar
        printf("\n");
        printMem("0x000000FF");
        printf("\n");
        getDateAndTime();
        while(1);
        printf("$>");


        //myScanf("%s %s", command, arg);
        //*command = 0;
        //*arg = 0;
        //scanf("%s %s", command, arg);
        //printf("Comando: %s\n\nArgumento: %s", command, arg);
        //execute(command, arg); 
    }
}

void execute(const char * command, const char * arg) {
    if(strcmp(command, "toto") == 0);
        //toto();
}

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

void shellWelcomeMsg(){
    printf("          -+#%%%%%%*-\n");              
    printf("        =@*-.......-*@=\n");            
    printf("       -@=....-+*#%%%@@#.\n");          
    printf("       @%...:%@+-:....:+%#:\n");        
    printf("   .:-+@+...*@*=:::.   .:+@=\n");       
    printf(" -@#++#@-...+@**+==------+@+\n");       
    printf(" %#:::#@-....%@#********#@#\n");        
    printf(" @#---@%-.....=*%%%%%%#**@-\n");        
    printf(".@*---@#-:..............:@=\n");        
    printf(".@*---@#--..............-@=\n");        
    printf(" @*---@#---.............+@-\n");        
    printf(" @%---@%-----:::.....:--*@.\n");        
    printf(" =@#+=%%----------------#@\n");         
    printf("  .-+*%@------#%%%@%#--=@*\n");         
    printf("      -@+----=@+ +@=---*@:\n");         
    printf("       @*----=@+ :@*===@#\n");          
    printf("      +@%##%%*:  :+##*+\n");           
    
}