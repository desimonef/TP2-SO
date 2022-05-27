// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include "shell.h"
#include "semLib.h"
#include "procLib.h"
#include "test_sync.h"
#include "sysCalls.h" 
#include "stdlib.h"
#include "memLib.h"

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID 101
#define SEM_SHELL 102

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  yieldProc();
  *p = aux;
}

void inc(int argc, char *argv[]){
  uint64_t i;
  int sem = atoi(argv[0]);
  int64_t value = atoi(argv[1]);
  int N = atoi(argv[2]  );

  if (sem && semOpen(SEM_ID, 1) == -1){
    printf("Error al crear proceso\n");
    return;
  }
  
  for (i = 0; i < N; i++){
    if (sem){
      if(semWait(SEM_ID) == -1){
        return;
      };
    } 
    slowInc(&global, value);
    if (sem) {
      if(semPost(SEM_ID) == -1){
        return;
      };
    }

  }

  if (sem) {
    if(semClose(SEM_ID) == -1){
      return;
    }
  } 
  printf("Final value: ");
  char buff[MAXLEN];
  printf(itoa(global, buff, 10));
  if(semPost(SEM_SHELL) == -1){
    return;
  }
}

void test_sync(){
  uint64_t i;

  global = 0;

  if(semOpen(SEM_SHELL, -TOTAL_PAIR_PROCESSES*2) == -1){
    return;
  }

  printf("CREATING PROCESSES...(WITH SEM)\n");
  int error;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char ** argv1 = malloc(4*sizeof(char));
    if(argv1 == NULL){
      return;
    }
    argv1[0] = "inc";
    argv1[1] = "1";
    argv1[2] = "1";
    argv1[3] = "1000";
    error = createProcess(&inc, 4, argv1, 0, NULL);
    if (error == -1)
    {
        printf("Error al crear el proceso");
        for(int p = 0; p < 4; p++){
          freeMem(argv1[p]);
        }
        freeMem(argv1);
        return ;
    }
   
    
    char **argv2 = malloc(4*sizeof(char));
    if(argv2 == NULL){
      return;
    }
    argv2[0] = "inc";
    argv2[1] = "1";
    argv2[2] = "-1";
    argv2[3] = "1000";
    error = createProcess(&inc, 4, argv2, 0, NULL);
    if (error == -1)
    {
        printf("Error al crear el proceso");
    }
    for(int k = 0; k < 4; k++){
      freeMem(argv1[k]);
      freeMem(argv2[k]);
    }
    freeMem(argv1);
    freeMem(argv2);
  }

  for (int i = 0; i < TOTAL_PAIR_PROCESSES*2; i++)
  {
    if(semWait(SEM_SHELL) == -1){
      return;
    };
  }

  if(semClose(SEM_SHELL) == -1){
    return;
  };
  
}

void test_no_sync(){
  uint64_t i;

  global = 0;

  if(semOpen(SEM_SHELL, -TOTAL_PAIR_PROCESSES*2) == -1){
    return;
  }
  printf("CREATING PROCESSES...(WITHOUT SEM)\n");
  int error;

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char *argv1[4] = {"inc", "1", "1", "1000"};
    error = createProcess(&inc, 4, argv1, 0, NULL);
    if(error == -1){
      printf("Error loading Process");
  
      return;
    }
    char *argv2[4] = {"inc", "0", "-1", "1000"};
    error = createProcess(&inc, 4, argv2, 0, NULL);
    if(error == -1){
      printf("Error loading Process");
  
      return;
    }
  }
  for (int i = 0; i < TOTAL_PAIR_PROCESSES*2 - 1; i++)
  {
    if(semWait(SEM_SHELL) == -1){
      return;
    };
  }

  if(semClose(SEM_SHELL) == -1){
    return;
  };
}
