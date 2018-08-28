#ifndef GRAPH_H
#define GRAPH_H

struct graph;
typedef struct graph graph_t;

struct isomorphic_group;
typedef struct isomorphic_group isomorphic_group_t;

struct graph_group;
typedef struct graph_group graph_group_t;

struct graph {
    isomorphic_group_t *group;
    char **face_matrix;
    char **dual_adjacency_matrix;
};

struct isomorphic_group {
    graph_group_t *group;
    int e;
    int r;
    char **adjacency_matrix;
    int num_graphs;
    graph_t *graphs;
};

struct graph_group {
    int v;
    int num_groups;
    isomorphic_group_t *groups;
};

int graph_init(graph_t *graph, isomorphic_group_t *group, int face_matrix_i);
int dual_graph_is_isomorphic(graph_t *a, graph_t *b);
int graph_free(graph_t *graph);

int isomorphic_group_init(isomorphic_group_t *isogroup, graph_group_t *group, int e, int adjacency_matrix_i);
int isomorphic_group_check_subgraphs(isomorphic_group_t *group, char **target_adjacency_matrix);
int isomorphic_group_is_planar(isomorphic_group_t *group);
int isomorphic_group_free(isomorphic_group_t *group);

int graph_group_init(graph_group_t *group, int v);
int graph_group_free(graph_group_t *group);

#endif
