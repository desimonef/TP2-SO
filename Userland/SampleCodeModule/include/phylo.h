#ifndef PHYLO_H
#define PHYLO_H

typedef enum
{
    THINKING = 1,
    EATING = 2,
    HUNGRY = 3
    
} State;

typedef struct Philosopher
{
    int pid;
    int sem;
    State State;

} Philosopher;

void runPhylos(int argc, char *argv[]);

#endif