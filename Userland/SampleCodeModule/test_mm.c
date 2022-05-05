// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_mm.h"
#include "stdlib.h"
#include "sysCalls.h"
//#include <string.h>


#define MAX_BLOCKS 128
#define MAX_MEMORY 1024 //Should be around 80% of memory managed by the MM
#define LOOPS 100000
#define NULL ((void *) 0)

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  int contador = LOOPS;

   printf("Testing...");
   printf("\n");

  while (contador){
    rq = 0;
    total = 0;
    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = (void *) syscall(MALLOC, mm_rqs[rq].size, 0, 0, 0, 0, 0); // TODO: Port this call as required
      if(mm_rqs[rq].address == NULL){
        printf("Malloc failed, exiting\n");
        return;
      }
      total += mm_rqs[rq].size;
      rq++;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          printf("Error!");


    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        syscall(FREE, (uint64_t) mm_rqs[i].address, 0, 0, 0, 0, 0);  // TODO: Port this call as required
    contador--;

    if(LOOPS >= 100000 && contador == LOOPS/2 ){
      printf("Yeah, it takes a while ;)");
      printf("\n");
    }  
  } 
  printf("Memory manager test OK");
  printf("\n");
}