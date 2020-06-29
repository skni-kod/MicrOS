#include "../stdio.h"
#include "../micros/micros_filesystem.h"

int rename(const char *oldname, const char *newname)
{
    if(micros_filesystem_rename_file(oldname, newname) == true)
    {
        return 0;
    }
    return 1;
}