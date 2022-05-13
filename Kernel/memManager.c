// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifdef MEM_LIST

#include "memManager.h"
#include "defs.h"

typedef long Align;
typedef union header Header;

union header {
    struct {
        union header *ptr;
        unsigned size;
    } data;
    Align x;
};

static Header * base;
static Header * startingNode = NULL;

unsigned long totalUnits;

void initMM(){
    ncPrint("Entering MM\n");
    totalUnits = (TOTALMEM + sizeof(Header) - 1) / sizeof(Header) + 1;
    startingNode = base = (Header *) MEM_START;
    startingNode->data.size = totalUnits;
    startingNode->data.ptr = startingNode; 
}

void * malloc(int size){
    if(size == 0)
        return NULL;

    unsigned long nunits = (size + sizeof(Header) - 1) / sizeof(Header) + 1;
    Header * currNode;
    Header * prevNode;
    prevNode = startingNode;
    currNode = prevNode->data.ptr;

    for(;;prevNode = currNode, currNode = currNode->data.ptr){
        if(currNode->data.size >= nunits){
            if(currNode->data.size == nunits)
                prevNode->data.ptr = currNode->data.ptr;
            else{
                currNode->data.size -= nunits;
                currNode += currNode->data.size;
                currNode->data.size = nunits;
            }
            startingNode = prevNode;
            return (void *)(currNode + 1);
        }
        if(currNode == startingNode)
            return NULL;
            
    }
}

void free(void * addr){
    if(addr == NULL || (((long) addr - (long) base) % sizeof(Header)) != 0)
        return;


    Header *freeBlock;
    Header *currNode;

    freeBlock = (Header *) addr - 1;

    if(freeBlock < base || freeBlock >= (base + totalUnits * sizeof(Header)))
        return;
    
    char isExternal = 0;

    currNode = startingNode;
    for(; !(freeBlock > currNode && freeBlock < currNode->data.ptr); currNode = currNode->data.ptr){
        if(freeBlock == currNode || freeBlock == currNode->data.ptr)
            return;
        if(currNode >= currNode->data.ptr && (freeBlock > currNode || freeBlock < currNode->data.ptr)){
            isExternal = 1;
            break;
        }
    }

    if(!isExternal && (currNode + currNode->data.size > freeBlock ||
    freeBlock + freeBlock->data.size > currNode->data.ptr))
        return ;

    if(freeBlock + freeBlock->data.size == currNode->data.ptr){
        freeBlock->data.size += currNode->data.ptr->data.size; 
        freeBlock->data.ptr = currNode->data.ptr->data.ptr;
    }
    else{
        freeBlock->data.ptr = currNode->data.ptr;
    }

    if(currNode + currNode->data.size == freeBlock){
        currNode->data.size += freeBlock->data.size;
        currNode->data.ptr = freeBlock->data.ptr;
    }
    else{
        currNode->data.ptr = freeBlock;
    }

    startingNode = currNode;
}

#endif