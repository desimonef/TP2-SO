// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef BUDDY

#include "memManager.h"
#include "defs.h"

typedef struct data{
    struct data * ptr;
    unsigned size;
} data;

static data * base;
static data * startingNode = NULL;

unsigned long totalUnits;

void initMM(){
    ncPrint("Entering MEM_LIST MM\n");
    totalUnits = (TOTALMEM + sizeof(data) - 1) / sizeof(data) + 1;
    startingNode = base = (data *) MEM_START;
    startingNode->size = totalUnits;
    startingNode->ptr = startingNode; 
}

void * malloc(int size){
    if(size == 0)
        return NULL;

    unsigned long nunits = (size + sizeof(data) - 1) / sizeof(data) + 1;
    data * currNode;
    data * prevNode;
    prevNode = startingNode;
    currNode = prevNode->ptr;

    for(;;prevNode = currNode, currNode = currNode->ptr){
        if(currNode->size >= nunits){
            if(currNode->size == nunits)
                prevNode->ptr = currNode->ptr;
            else{
                currNode->size -= nunits;
                currNode += currNode->size;
                currNode->size = nunits;
            }
            startingNode = prevNode;
            return (void *)(currNode + 1);
        }
        if(currNode == startingNode)
            return NULL;
            
    }
}

void free(void * addr){
    if(addr == NULL || (((long) addr - (long) base) % sizeof(data)) != 0)
        return;
        
    data *freeBlock;
    data *currNode;

    freeBlock = (data *) addr - 1;

    if(freeBlock < base || freeBlock >= (base + totalUnits * sizeof(data)))
        return;
    
    char isExternal = 0;

    currNode = startingNode;
    for(; !(freeBlock > currNode && freeBlock < currNode->ptr); currNode = currNode->ptr){
        if(freeBlock == currNode || freeBlock == currNode->ptr)
            return;
        if(currNode >= currNode->ptr && (freeBlock > currNode || freeBlock < currNode->ptr)){
            isExternal = 1;
            break;
        }
    }

    if(!isExternal && (currNode + currNode->size > freeBlock ||
    freeBlock + freeBlock->size > currNode->ptr))
        return ;

    // Uno por derecha
    if(freeBlock + freeBlock->size == currNode->ptr){
        freeBlock->size += currNode->ptr->size; 
        freeBlock->ptr = currNode->ptr->ptr;
    }
    else{
        freeBlock->ptr = currNode->ptr;
    }

    // Uno por izquierda
    if(currNode + currNode->size == freeBlock){
        currNode->size += freeBlock->size;
        currNode->ptr = freeBlock->ptr;
    }
    else{
        currNode->ptr = freeBlock;
    }
    startingNode = currNode;
}

void memDump(){
    ncPrint("Memdump!\n");
}

#endif