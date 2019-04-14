#ifndef SETJMP_H
#define SETJMP_H

#include <stdint.h>

//! Type to hold information to restore calling environment. Contains ebx, esi, edi, ebp, return address.
typedef uint32_t jmp_buf[5];

#ifdef __cplusplus
extern "C" {
#endif

//! Long jump.
/*!
    Restores the environment to the state indicated by env, evaluating the setjmp expression that filled env as val. 
    \param env Object of type jmp_buf filled by a previous call to setjmp that contains information to restore the environment to that point.
    \param val Value to which the setjmp expression evaluates. If this is zero, the expression evaluates as 1.
*/
extern void longjmp(jmp_buf env, int val);

//! Save calling environment for long jump.
/*!
    Fills env with information about the current state of the calling environment in that point of code execution, so that it can be restored by a later call to longjmp. 
    \param env Object of type jmp_buf where the environment information is stored.
    \return This macro may return more than once: A first time, on its direct invocation; In this case it always returns zero. When longjmp is called with the information set to env, the macro returns again; this time it returns the value passed to longjmp as second argument if this is different from zero, or 1 if it is zero.
*/
extern int setjmp(jmp_buf env);

#ifdef __cplusplus
}
#endif

#endif