#include "core/core.h"

char current_partition;

int main(int argc, char *argv[])
{
    current_partition = argv[1][0];
    core_run();
    
    return 0;
}