#include "test.h"

int test(int a){
    #ifdef KERNEL
        a = a * a;
    #endif
    
    return a;
}