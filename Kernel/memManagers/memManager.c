// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef BUDDY

#include "memManager.h"
#include "defs.h"

typedef struct data{
    struct data * ptr;
    unsigned size;
} data;

unsigned long quantity;

static data * base;
static data * start = NULL;

void initMM(){
    ncPrint("Entering MEM_LIST MM\n");
    start = base = (data *) MEM_START;
    quantity = (TOTALMEM + sizeof(data) - 1) / sizeof(data) + 1;
    start->size = quantity;
    start->ptr = start; 
}

void * malloc(int size){
    if(size == 0)
        return NULL;

    unsigned long num = (size + sizeof(data) - 1) / sizeof(data) + 1;
    data * current;
    data * last;
    last = start;
    current = last->ptr;

    for(;;last = current, current = current->ptr){
        if(current->size >= num){
            if(current->size == num)
                last->ptr = current->ptr;
            else{
                current->size -= num;
                current += current->size;
                current->size = num;
            }
            start = last;
            return (void *)(current + 1);
        }
        if(current == start)
            return NULL;
            
    }
}

void free(void * addr){
    if(addr == NULL || (((long) addr - (long) base) % sizeof(data)) != 0)
        return;
        
    data *freeBlock;
    data *current;

    char outer = 0;

    freeBlock = (data *) addr - 1;

    if(freeBlock < base || freeBlock >= (base + quantity * sizeof(data)))
        return;

    current = start;
    for(; !(freeBlock > current && freeBlock < current->ptr); current = current->ptr){
        if(freeBlock == current || freeBlock == current->ptr)
            return;
        if(current >= current->ptr && (freeBlock > current || freeBlock < current->ptr)){
            outer = 1;
            break;
        }
    }

    if(!outer && (current + current->size > freeBlock ||
    freeBlock + freeBlock->size > current->ptr))
        return ;

    
    if(freeBlock + freeBlock->size == current->ptr){
        freeBlock->size += current->ptr->size; 
        freeBlock->ptr = current->ptr->ptr;
    }
    else{
        freeBlock->ptr = current->ptr;
    }

    
    if(current + current->size == freeBlock){
        current->size += freeBlock->size;
        current->ptr = freeBlock->ptr;
    }
    else{
        current->ptr = freeBlock;
    }
    start = current;
}

void memDump(){
    //ncPrint("Memdump!\n");
    ncNewline();
    ncNewline();
    ncPrint("-----Estado de memoria-----");
    ncNewline();
    ncPrint("Memoria total: ");
    int total =  quantity*sizeof(data);
    ncPrintDec((uint64_t) total);
    ncPrint(" B");
    ncNewline();
    ncPrint("---------------------------");
    ncNewline();
    ncPrint("Memoria libre: ");

    data * auxPointer = start;
    data * auxPointer2 = start;
    int sum = 0;
    int flag = 1;
    while(auxPointer != auxPointer2 || flag){
        flag = 0;
        sum += auxPointer->size;
        auxPointer = auxPointer->ptr;
    }

    ncPrintDec((uint64_t) sum);
    ncNewline();
    ncPrint("---------------------------");
    ncNewline();
    ncPrint("Memoria ocupada: ");
    ncPrintDec((uint64_t) total - (uint64_t) sum);
    ncNewline();
    ncPrint("---------------------------");
    ncNewline();
}

#endif
