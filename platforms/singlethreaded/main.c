#include <errno.h>
#include <stdio.h>
#include <igndg.h>

int main(int argc, char const **argv) {
    context_t ctx;
    work_unit_t work;
    if (logic_init(&ctx) < 0) {
        perror("logic_init");
        return 1;
    }
    while (1) {
        if (logic_alloc_work_unit(&ctx, &work) < 0) {
            perror("logic_alloc_work_unit");
            if (logic_cleanup(&ctx) < 0) {
                perror("logic_cleanup");
            }
            return 1;
        }
        if (logic_do_work_unit(&work) < 0) {
            if (errno == 0) {
                break;
            } else {
                perror("logic_do_work_unit");
                if (logic_free_work_unit(&ctx, &work) < 0) {
                    perror("logic_free_work_unit");
                }
                if (logic_cleanup(&ctx) < 0) {
                    perror("logic_cleanup");
                }
                return 1;
            }
        }
        if (logic_free_work_unit(&ctx, &work) < 0) {
            perror("logic_free_work_unit");
            if (logic_cleanup(&ctx) < 0) {
                perror("logic_cleanup");
            }
            return 1;
        }
    }
    if (logic_print_solution(&ctx) < 0) {
        perror("logic_print_solution");
        if (logic_cleanup(&ctx) < 0) {
            perror("logic_cleanup");
        }
        return 1;
    }
    if (logic_cleanup(&ctx) < 0) {
        perror("logic_cleanup");
        return 1;
    }
    return 0;
}
