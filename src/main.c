#include <stdio.h>
#include <time.h>

#include "common.h"
#include "scan.h"

static char* read_file(const char* path);

int main(int argc, char *argv[])
{
    struct timespec t0, t1, t2, t3;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
    const char* code = read_file("/home/ostkaka/Desktop/test.js");//"int sten = 3.14;";

    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    vec_token_t tokens = scan(code);

    clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
    for (int i = 0; i < tokens.length; i++) {
        token_t token = tokens.data[i];
        printf("%.*s ", token.len, code + token.index);
    }


    double diff0 = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_nsec - t0.tv_nsec) / 1000000.0;
    double diff1 = (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_nsec - t1.tv_nsec) / 1000000.0;
    printf("read %f ms\n", diff0);
    printf("scan %f ms\n", diff1);
    return 0;
}


static char* read_file(const char* path) {
    FILE* file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    void* data = malloc(file_size);
    rewind(file);
    fread(data, 1, file_size, file);
    fclose(file);
    //if (size) size = file_size;
    return data;
}
