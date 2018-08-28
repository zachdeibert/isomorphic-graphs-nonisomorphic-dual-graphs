#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <list.h>

char K33[6][6] = {
    { 0, 1, 0, 1, 0, 1 },
    { 1, 0, 1, 0, 1, 0 },
    { 0, 1, 0, 1, 0, 1 },
    { 1, 0, 1, 0, 1, 0 },
    { 0, 1, 0, 1, 0, 1 },
    { 1, 0, 1, 0, 1, 0 }
};

char K5[5][5] = {
    { 0, 1, 1, 1, 1 },
    { 1, 0, 1, 1, 1 },
    { 1, 1, 0, 1, 1 },
    { 1, 1, 1, 0, 1 },
    { 1, 1, 1, 1, 0 }
};

int graph_init(graph_t *graph, isomorphic_group_t *group, int face_matrix_i) {
    graph->group = group;
    graph->face_matrix = (char **) malloc(sizeof(char *) * group->group->v);
    if (!graph->face_matrix) {
        int tmp = errno;
        perror("malloc");
        errno = tmp;
        return -1;
    }
    for (int i = 0; i < group->group->v; ++i) {
        graph->face_matrix[i] = (char *) malloc(sizeof(char) * group->e);
        if (!graph->face_matrix[i]) {
            int tmp = errno;
            perror("malloc");
            for (--i; i >= 0; --i) {
                free(graph->face_matrix[i]);
            }
            free(graph->face_matrix);
            errno = tmp;
            return -1;
        }
    }
    graph->dual_adjacency_matrix = (char **) malloc(sizeof(char *) * group->r);
    if (!graph->dual_adjacency_matrix) {
        int tmp = errno;
        perror("malloc");
        for (int i = 0; i < group->group->v; ++i) {
            free(graph->face_matrix[i]);
        }
        free(graph->face_matrix);
        errno = tmp;
        return -1;
    }
    for (int i = 0; i < group->r; ++i) {
        graph->dual_adjacency_matrix[i] = (char *) malloc(sizeof(char) * group->r);
        if (!graph->dual_adjacency_matrix[i]) {
            int tmp = errno;
            perror("malloc");
            for (--i; i >= 0; --i) {
                free(graph->dual_adjacency_matrix[i]);
            }
            free(graph->dual_adjacency_matrix);
            for (i = 0; i < group->group->v; ++i) {
                free(graph->face_matrix[i]);
            }
            free(graph->face_matrix);
            errno = tmp;
            return -1;
        }
    }
    // TODO
    errno = ENOSYS;
    return -1;
}

int dual_graph_is_isomorphic(graph_t *a, graph_t *b) {
    // TODO
    errno = ENOSYS;
    return -1;
}

int graph_free(graph_t *graph) {
    for (int i = 0; i < graph->group->group->v; ++i) {
        free(graph->face_matrix[i]);
    }
    free(graph->face_matrix);
    for (int i = 0; i < graph->group->r; ++i) {
        free(graph->dual_adjacency_matrix[i]);
    }
    free(graph->dual_adjacency_matrix);
    return 0;
}

int isomorphic_group_init(isomorphic_group_t *isogroup, graph_group_t *group, int e, int adjacency_matrix_i) {
    isogroup->group = group;
    isogroup->e = e;
    isogroup->r = 2 + e - group->v;
    isogroup->adjacency_matrix = (char **) malloc(sizeof(char *) * group->v);
    if (!isogroup->adjacency_matrix) {
        int tmp = errno;
        perror("malloc");
        errno = tmp;
        return -1;
    }
    for (int i = 0; i < group->v; ++i) {
        isogroup->adjacency_matrix[i] = (char *) malloc(sizeof(char) * group->v);
        if (!isogroup->adjacency_matrix[i]) {
            int tmp = errno;
            perror("malloc");
            for (--i; i >= 0; --i) {
                free(isogroup->adjacency_matrix[i]);
            }
            free(isogroup->adjacency_matrix);
            errno = tmp;
            return -1;
        }
    }
    // TODO
    errno = ENOSYS;
    return -1;
}

int isomorphic_group_check_subgraphs(isomorphic_group_t *group, char **target_adjacency_matrix) {
    // TODO
    errno = ENOSYS;
    return -1;
}

int isomorphic_group_is_planar(isomorphic_group_t *group) {
    int res = isomorphic_group_check_subgraphs(group, (char **) K33);
    if (res < 0) {
        int tmp = errno;
        perror("isomorphic_group_check_subgraphs");
        errno = tmp;
        return -1;
    } else if (res) {
        res = isomorphic_group_check_subgraphs(group, (char **) K5);
        if (res < 0) {
            int tmp = errno;
            perror("isomorphic_group_check_subgraphs");
            errno = tmp;
            return -1;
        } else {
            return res;
        }
    } else {
        return 0;
    }
}

int isomorphic_group_free(isomorphic_group_t *group) {
    int ret = 0;
    for (int i = 0; i < group->num_graphs; ++i) {
        if (graph_free(group->graphs + i) < 0) {
            int tmp = errno;
            perror("graph_free");
            errno = tmp;
            ret = -1;
        }
    }
    for (int i = 0; i < group->group->v; ++i) {
        free(group->adjacency_matrix[i]);
    }
    free(group->adjacency_matrix);
    free(group->graphs);
    return ret;
}

int graph_group_init(graph_group_t *group, int v) {
    group->v = v;
    list_node_t list = {
        .previous = NULL,
        .next = NULL,
        .value = NULL
    };
    isomorphic_group_t *isogroup = (isomorphic_group_t *) malloc(sizeof(isomorphic_group_t));
    if (!isogroup) {
        int tmp = errno;
        perror("malloc");
        errno = tmp;
        return -1;
    }
    int max_e = v * (v - 1) / 2;
    for (int e = 3; e <= max_e; ++e) {
        for (int i = 0; ; ++i) {
            int res = isomorphic_group_init(isogroup, group, e, i);
            if (res < 0) {
                int tmp = errno;
                perror("isomorphic_group_init");
                if (list.value) {
                    list_clear_and_free(&list);
                }
                errno = tmp;
                return -1;
            } else if (res) {
                res = isomorphic_group_is_planar(isogroup);
                if (res < 0) {
                    int tmp = errno;
                    perror("isomorphic_group_is_planar");
                    if (list.value) {
                        list_clear_and_free(&list);
                    }
                    errno = tmp;
                    return -1;
                } else if (res) {
                    list_append(&list, isogroup);
                    isogroup = (isomorphic_group_t *) malloc(sizeof(isomorphic_group_t));
                    if (!isogroup) {
                        int tmp = errno;
                        perror("malloc");
                        if (list.value) {
                            list_clear_and_free(&list);
                        }
                        errno = tmp;
                        return -1;
                    }
                }
            } else {
                break;
            }
        }
    }
    free(isogroup);
    group->num_groups = list_length(&list);
    group->groups = (isomorphic_group_t *) malloc(sizeof(isomorphic_group_t) * group->num_groups);
    if (!group->groups) {
        int tmp = errno;
        perror("malloc");
        list_clear_and_free(&list);
        errno = tmp;
        return -1;
    }
    list_to_array(&list, group->groups, sizeof(isomorphic_group_t));
    list_clear_and_free(&list);
    return 0;
}

int graph_group_free(graph_group_t *group) {
    int ret = 0;
    for (int i = 0; i < group->num_groups; ++i) {
        if (isomorphic_group_free(group->groups + i) < 0) {
            int tmp = errno;
            perror("isomorphic_group_free");
            errno = tmp;
            ret = -1;
        }
    }
    free(group->groups);
    return ret;
}
