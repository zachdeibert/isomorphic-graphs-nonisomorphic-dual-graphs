#include <errno.h>
#include <stdio.h>
#include <igndg.h>

int main(int argc, char const **argv) {
    void *ctx = logic_init();
    if (ctx == 0) {
        perror("logic_init");
        return 1;
    }
    int res;
    do {
        res = logic_loop(ctx);
        if (res < 0) {
            perror("logic_loop");
            if (logic_cleanup(ctx) < 0) {
                perror("logic_cleanup");
            }
            return 1;
        }
    } while (res);
    if (logic_cleanup(ctx) < 0) {
        perror("logic_cleanup");
        return 1;
    }
    return 0;
}
