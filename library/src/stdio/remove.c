#include "../stdio.h"
#include "../micros/micros_filesystem.h"

int remove(const char *filename)
{
    if(micros_filesystem_delete_file(filename) == true)
    {
        return 0;
    }
    return 1;
}