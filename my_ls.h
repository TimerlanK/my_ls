#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct
{
    int a;
    int t;
} options_struct;

typedef struct
{
    char name[100];
    int type;
    long long mod_time_sec;
    long long mod_time_nsec;
} file_info_struct;


