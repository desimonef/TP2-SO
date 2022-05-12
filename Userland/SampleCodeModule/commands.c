#include "sysCalls.h"
#include "stdlib.h"
#include "test_mm.h"
//#include "test_prio.h"
#include "test_processes.h"
//#include "test_sync.h"

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
    int c;
    while ((c = getchar()) != -1)
            putchar(c);
}

void wc(int argc, char ** argv){
    int c;
    int count = 1;

    while ((c = getchar()) != -1)
    {
        putchar(c);
        if ((char)c == '\n')
                count++;
    }
    print("\nNumber of lines: %d\n", count);
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
    int c;
    while ((c = getchar()) != -1)
    {
        if (!check_vowel(c))
                putchar(c);
    }
}

void pipe(int argc, char ** argv){
    syscall(PRINT_PIPES, 0, 0, 0, 0, 0, 0);
}

void test_prio(int argc, char ** argv){
    
};

void testMM(int argc, char ** argv){
    test_mm();
};

void testProc(int argc, char ** argv){
    test_processes();
};

void testSync(int argc, char ** argv){};
void testNosync(int argc, char ** argv){};