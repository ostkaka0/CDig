#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Failed to open file '%s'!", path);
        return -1;
    }



    fclose(file);
    return 0;
}
