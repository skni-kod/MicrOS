#ifndef PARTITIONS_H
#define PARTITIONS_H

#include <stdbool.h>
#include "filesystems/fat/fat.h"
#include "partition.h"
#include "klibrary/kvector.h"

void partitions_init();
partition *partitions_get(char *path);

#endif