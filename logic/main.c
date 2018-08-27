#include <stdio.h>
#include <stdlib.h>
#include <igndg.h>

void *logic_init() {
    puts("logic_init");
    void *ctx = malloc(4);
    if (!ctx) {
        return 0;
    }
    return ctx;
}

int logic_loop(void *ctx) {
    puts("logic_loop");
    int *val = (int *) ctx;
    return ++*val < 4;
}

int logic_cleanup(void *ctx) {
    puts("logic_cleanup");
    free(ctx);
    return 0;
}
