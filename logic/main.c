#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <igndg.h>

int logic_init(context_t *ctx) {
    if (graph_group_init(&ctx->graphs, 3) < 0) {
        int tmp = errno;
        perror("graph_group_init");
        errno = tmp;
        return -1;
    }
    ctx->group = ctx->graphs.groups;
    ctx->idx[0] = -1;
    ctx->idx[1] = 0;
    return 0;
}

int logic_alloc_work_unit(context_t *ctx, work_unit_t *work) {
    while (1) {
        if (++ctx->idx[0] >= ctx->group[0]->num_graphs) {
            ctx->idx[0] = 0;
            if (++ctx->idx[1] >= ctx->group[0]->num_graphs) {
                ctx->idx[1] = 0;
                if (++ctx->group >= ctx->graphs.groups + ctx->graphs.num_groups) {
                    int v = ctx->graphs.v;
                    if (graph_group_free(&ctx->graphs) < 0) {
                        int tmp = errno;
                        perror("graph_group_free");
                        errno = tmp;
                        return -1;
                    }
                    if (graph_group_init(&ctx->graphs, v + 1) < 0) {
                        int tmp = errno;
                        perror("graph_group_init");
                        errno = tmp;
                        return -1;
                    }
                    ctx->group = ctx->graphs.groups;
                }
            }
        }
        if (ctx->idx[0] != ctx->idx[1]) {
            work->graphs[0] = ctx->group[0]->graphs[ctx->idx[0]];
            work->graphs[1] = ctx->group[0]->graphs[ctx->idx[1]];
            work->solutions = ctx->solutions;
            return 0;
        }
    }
}

int logic_do_work_unit(work_unit_t *work) {
    int res = dual_graph_is_isomorphic(work->graphs[0], work->graphs[1]);
    if (res < 0) {
        int tmp = errno;
        perror("dual_graph_is_isomorphic");
        errno = tmp;
        return -1;
    } else if (res) {
        return 0;
    } else {
        work->solutions[0] = work->graphs[0];
        work->solutions[1] = work->graphs[1];
        errno = 0;
        return -1;
    }
}

int logic_free_work_unit(context_t *ctx, work_unit_t *work) {
    return 0;
}

int logic_print_status(context_t *ctx) {
    puts("Problem status");
    puts("======");
    puts("");
    printf("Vertex Count = %d\n", ctx->graphs.v);
    printf("Edge Count   = %d\n", ctx->group[0]->e);
    printf("Region Count = %d\n", ctx->group[0]->r);
    puts("");
    puts("");
    return 0;
}

int logic_print_solution(context_t *ctx) {
    puts("Problem solution found!");
    puts("======");
    puts("");
    puts("Graph Properties:");
    printf("  V = %d\n", ctx->graphs.v);
    printf("  E = %d\n", ctx->group[0]->e);
    printf("  R = %d\n", ctx->group[0]->r);
    puts("Adjacency Matrix:");
    for (int y = 0; y < ctx->graphs.v; ++y) {
        printf("  [ ");
        for (int x = 0; x < ctx->graphs.v; ++x) {
            printf("%d ", ctx->group[0]->adjacency_matrix[x][y]);
        }
        puts("]");
    }
    puts("");
    puts("Graph A Face Matrix:");
    for (int y = 0; y < ctx->solutions[0]->group->r; ++y) {
        printf("  [ ");
        for (int x = 0; x < ctx->solutions[0]->group->group->v; ++x) {
            printf("%d ", ctx->solutions[0]->face_matrix[x][y]);
        }
        puts("]");
    }
    puts("");
    puts("Graph B Face Matrix:");
    for (int y = 0; y < ctx->solutions[1]->group->r; ++y) {
        printf("  [ ");
        for (int x = 0; x < ctx->solutions[1]->group->group->v; ++x) {
            printf("%d ", ctx->solutions[1]->face_matrix[x][y]);
        }
        puts("]");
    }
    puts("");
    puts("");
    return 0;
}

int logic_cleanup(context_t *ctx) {
    if (graph_group_free(&ctx->graphs) < 0) {
        int tmp = errno;
        perror("graph_group_free");
        errno = tmp;
        return -1;
    }
    return 0;
}
