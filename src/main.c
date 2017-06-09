

#include <stdio.h>
#define _POSIX_C_SOURCE >= 199309
#include <time.h>

#include "common.h"
#include "scan.h"

static char* read_file(const char* path);

int main(int argc, char *argv[])
{
    struct timespec t0, t1, t2, t3;
    timespec_get(&t0, TIME_UTC);
    const char* code = read_file("/home/ost/Desktop/test.js");//"int sten = 3.14;";
    if (!code)

    timespec_get(&t0, TIME_UTC);
    vec_token_t tokens = scan(code);

    timespec_get(&t2, TIME_UTC);
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
    if (!file) return NULL;
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    void* data = malloc(file_size);
    rewind(file);
    fread(data, 1, file_size, file);
    fclose(file);
    //if (size) size = file_size;
    return data;
}
