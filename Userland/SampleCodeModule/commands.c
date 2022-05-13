// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "sysCalls.h"
#include "stdlib.h"
#include "test_mm.h"
//#include "test_prio.h"
#include "test_processes.h"
#include "test_sync.h"

int check_vowel(char a);


void mem(int argc, char ** argv){
    syscall(PRINT_MEM, 0, 0, 0, 0, 0, 0);
}

void ps(int argc, char ** argv){
    syscall(PS, 0, 0, 0, 0, 0, 0);
}

int ticksElapsed()
{
      return syscall(TICKS_ELAPSED, 0, 0, 0, 0, 0, 0);
}


void waitCycles(int cycles)
{
      int goal = ticksElapsed() + cycles;
      while (ticksElapsed() < goal)
            ;
}

void loop(int argc, char ** argv){
    long pid = syscall(CURRENT_P, 0, 0, 0, 0, 0, 0);
    while (1)
    {
        waitCycles(10);
        printf("%d\n", pid);
    }
}

void kill(int argc, char ** argv){
    int PID = atoi(argv[1]);
    if(PID == -1){
        printf("Invalid PID\n");
        return;
    }
    syscall(KILL_P, PID, 0, 0, 0, 0, 0);
}

void nice(int argc, char ** argv){
    int PID = atoi(argv[1]);
    int prio = atoi(argv[2]);
    if(PID == -1 || prio == -1){
        printf("Invalid parameters. Try again.\n");
        return;
    }
    syscall(NICE, PID, prio, 0, 0, 0, 0);
}

void block(int argc, char ** argv){
    int PID = atoi(argv[1]);
    if(PID == -1){
        printf("Invalid PID\n");
        return;
    }
    syscall(BLOCK_P, PID, 0, 0, 0, 0, 0);
}

void unblock(int argc, char ** argv){
    int PID = atoi(argv[1]);
    if(PID == -1){
        printf("Invalid PID\n");
        return;
    }
    syscall(UNBLOCK_P, PID, 0, 0, 0, 0, 0);
}

void sem(int argc, char ** argv){
    syscall(PRINT_SEMS, 0, 0, 0, 0, 0, 0);
}

void cat(int argc, char ** argv){
    // int c;
    // while ((c = getchar()) != -1)
    //         putchar(c);

    int i = 0;
    while(argv[1][i] != '\0' && argv[1][i] != '\n')
        putchar(argv[1][i++]);
}

void wc(int argc, char ** argv){
    // int c;
    // int count = 1;

    // while ((c = getchar()) != -1)
    // {
    //     putchar(c);
    //     if ((char)c == '\n')
    //             count++;
    // }
    // printf("\nNumber of lines: %d\n", count);

    int count = 1;
    int i = 0;
    while(argv[1][i] != '\0' && argv[1][i] != '\n'){
        putchar(argv[1][i++]);
        if(argv[1][i++] == '\n')
            count++;
    }
    printf("\nNumber of lines: %d\n", count);
}

int check_vowel(char a)
{
      if (a >= 'A' && a <= 'Z')
            a = a + 'a' - 'A'; /* Converting to lower case or use a = a + 32 */

      if (a == 'a' || a == 'e' || a == 'i' || a == 'o' || a == 'u')
            return 1;

      return 0;
}

void filter(int argc, char ** argv){
    printf("\nargv[1] = ");
    printf(argv[1]);
    printf("\n");
    int i = 0;
    while (argv[1][i] != '\0' && argv[1][i] != '\n')
    {
        if (!check_vowel(argv[1][i]))
                putchar(argv[1][i]);
        i++;
    }

}

void pipe(int argc, char ** argv){
    syscall(PRINT_PIPES, 0, 0, 0, 0, 0, 0);
}

void testPrio(int argc, char ** argv){
    //test_prio();
};

void testMM(int argc, char ** argv){
    test_mm();
};

void testProc(int argc, char ** argv){
    test_processes();
};

void testSync(int argc, char ** argv){
    test_sync();
};
void testNosync(int argc, char ** argv){
    test_no_sync();
};