// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "sysCalls.h"
#include "stdlib.h"
#include "semLib.h"
#include "procLib.h"
#include "memLib.h"
//Tanenbaum's based implementation

#define MAX_PHILOS 8
#define BASE_PHILOS 4
#define MUTEX_ID 999
#define BASE_SEM_ID 1000

typedef enum
{
    THINKING = 1,
    EATING = 2,
    HUNGRY = 3,
} State;

typedef struct Philosopher
{
    int pid;
    int sem;
    State State;
} Philosopher;

Philosopher *philos[MAX_PHILOS];
static int actualPhilosopherCount = 0;
static int tableMutex;
static int problemRunning;

#define RIGHT(i) ((i) + 1) % (actualPhilosopherCount)                         
#define LEFT(i) ((i) + actualPhilosopherCount - 1) % (actualPhilosopherCount) 

void philo(int argc, char *argv[]);
void takeForks(int i);
void placeForks(int i);
void test(int i);
int addPhilosopher();
int removePhilosopher();
void printfTable();

void philo(int argc, char *argv[])
{
    int idx = atoi(argv[1]);
    while (problemRunning)
    {
        takeForks(idx);
        sleep(1);
        placeForks(idx);
        sleep(1);
    }
}

void takeForks(int i)
{
    semWait(tableMutex);
    philos[i]->State = HUNGRY;
    test(i);
    semPost(tableMutex);
    semWait(philos[i]->sem);
}

void placeForks(int i)
{
    semWait(tableMutex);
    philos[i]->State = THINKING;
    test(LEFT(i));
    test(RIGHT(i));
    semPost(tableMutex);
}

void test(int i)
{
    if (philos[i]->State == HUNGRY && philos[LEFT(i)]->State != EATING && philos[RIGHT(i)]->State != EATING)
    {
        philos[i]->State = EATING;
        semPost(philos[i]->sem);
    }
}

void runPhylos(int argc, char *argv[])
{
    problemRunning = 1;
    tableMutex = semOpen(MUTEX_ID, 1);
    printf("Problema de los filosofos.\n");
    printf("Instrucciones:\n- 'a': Agrega un filosofo\n- 'b': Borra un filosofo\n- 'q': Cierra el programa\nComencemos :)\n\n");

    sleep(2);

    for (int i = 0; i < BASE_PHILOS; i++)
        addPhilosopher();
    char *args[] = {"printfTable"};
    int printfTablePid = createProcess(&printfTable, 1, args, BG, NULL);
    while (problemRunning)
    {

        char key = getchar();
        switch (key)
        {
        case 'a':
            if (addPhilosopher() == -1)
                printf("No se puede agregar (maximo 8)\n");
            else
                printf("Agregando filósofo...\n");
            break;
        case 'b':
            if (removePhilosopher() == -1)
                printf("No se puede remover (minimo 4)\n");
            else
                printf("Removiendo filosofo...\n");
            break;
        case 'q':
            printf("\nFinalizando. Hasta la proxima :)\n\n");
            problemRunning = 0;
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < actualPhilosopherCount; i++)
    {
        semClose(philos[i]->sem);
        killProc(philos[i]->pid);
        freeMem(philos[i]);
    }
    actualPhilosopherCount = 0;
    killProc(printfTablePid);
    semClose(MUTEX_ID);
}

int addPhilosopher()
{
    if (actualPhilosopherCount == MAX_PHILOS)
        return -1;

    semWait(tableMutex);
    Philosopher *auxPhilo = malloc(sizeof(Philosopher));
    if (auxPhilo == NULL)
        return -1;
    auxPhilo->State = THINKING;
    auxPhilo->sem = semOpen(BASE_SEM_ID + actualPhilosopherCount, 1);
    char buffer[3];
    char *name[] = {"philosopher", itoa(actualPhilosopherCount, buffer, 10)};
    auxPhilo->pid = createProcess(&philo, 2, name, BG, NULL);
    philos[actualPhilosopherCount++] = auxPhilo;
    semPost(tableMutex);
    return 0;
}

int removePhilosopher()
{
    if (actualPhilosopherCount == BASE_PHILOS)
    {
        return -1;
    }

    actualPhilosopherCount--;
    Philosopher *chosenPhilo = philos[actualPhilosopherCount];
    semClose(chosenPhilo->sem);
    killProc(chosenPhilo->pid);
    freeMem(chosenPhilo);
    semPost(tableMutex);

    return 0;
}

void printfTable(int argc, char *argv[])
{
    while (problemRunning)
    {
        semWait(tableMutex);
        for (int i = 0; i < actualPhilosopherCount; i++)
        {
            philos[i]->State == EATING ? putchar('E') : putchar('.');
            putchar(' ');
        }
        putchar('\n');
        semPost(tableMutex);
        yieldProc();
        //TODO: void yield();
    }
}
