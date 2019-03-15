#ifndef SIGNAL_H
#define SIGNAL_H

#define SIGABRT 0
#define SIGFPE 1
#define SIGILL 2
#define SIGINT 3
#define SIGSEGV 4
#define SIGTERM 5

typedef int32_t sig_atomic_t;

void (*signal(int sig, void (*func)(int)))(int);
int raise(int sig);

#endif