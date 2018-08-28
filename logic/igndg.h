#ifndef IGNDG_H
#define IGNDG_H
#include <graph.h>

struct context;
typedef struct context context_t;

struct work_unit;
typedef struct work_unit work_unit_t;

struct context {
    graph_group_t graphs;
    isomorphic_group_t **group;
    int idx[2];
    graph_t *solutions[2];
};

struct work_unit {
    graph_t *graphs[2];
    graph_t **solutions;
};

int logic_init(context_t *ctx);
int logic_alloc_work_unit(context_t *ctx, work_unit_t *work);
int logic_do_work_unit(work_unit_t *work);
int logic_free_work_unit(context_t *ctx, work_unit_t *work);
int logic_print_status(context_t *ctx);
int logic_print_solution(context_t *ctx);
int logic_cleanup(context_t *ctx);

#endif
