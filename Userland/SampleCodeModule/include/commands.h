#ifndef COMMANDS
#define COMMANDS

void mem(int argc, char ** argv);

void loop(int argc, char ** argv);

void ps(int argc, char ** argv);
void kill(int argc, char ** argv);
void nice(int argc, char ** argv);
void block(int argc, char ** argv);
void unblock(int argc, char ** argv);

void sem(int argc, char ** argv);

void cat(int argc, char ** argv);
void wc(int argc, char ** argv);
void filter(int argc, char ** argv);

void pipe(int argc, char ** argv);

void testPrio(int argc, char ** argv);
void testMM(int argc, char ** argv);
void testProc(int argc, char ** argv);
void testSync(int argc, char ** argv);
void testNosync(int argc, char ** argv);

void helpWrp(int argc, char ** argv);
void regWrp(int argc, char ** argv);
void dumpWrp(int argc, char ** argv);
void datetimeWrp(int argc, char ** argv);
void divZeroWrp(int argc, char ** argv);
void opCodeWrp(int argc, char ** argv);
void clear(int argc, char ** argv);

#endif

