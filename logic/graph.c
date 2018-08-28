#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graph.h>
#include <list.h>
#include <snoob.h>

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
        graph->face_matrix[i] = (char *) malloc(sizeof(char) * group->r);
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
    unsigned long long pattern = (1 << (group->e << 1)) - 1;
    int max = group->r * group->group->v;
    while (face_matrix_i >= 0) {
        if (pattern >= (1 << max)) {
            for (int i = 0; i < group->r; ++i) {
                free(graph->dual_adjacency_matrix[i]);
            }
            free(graph->dual_adjacency_matrix);
            for (int i = 0; i < group->group->v; ++i) {
                free(graph->face_matrix[i]);
            }
            free(graph->face_matrix);
            return 0;
        }
        unsigned long long tmp = pattern;
        for (int x = 0; x < group->group->v; ++x) {
            for (int y = 0; y < group->e; ++y) {
                graph->face_matrix[x][y] = tmp & 1;
                tmp >>= 1;
            }
        }
        int bad = 0;
        for (int a = 0; a < group->r; ++a) {
            for (int b = 0; b < group->r; ++b) {
                if (a != b) {
                    int good = 0;
                    for (int x = 0; x < group->group->v; ++x) {
                        if (graph->face_matrix[x][a] != graph->face_matrix[x][b]) {
                            good = 1;
                            break;
                        }
                    }
                    if (!good) {
                        bad = 1;
                        break;
                    }
                }
            }
            if (bad) {
                break;
            }
        }
        if (!bad) {
            --face_matrix_i;
        }
        pattern = snoob(pattern);
    }
    for (int x = 0; x < group->r; ++x) {
        for (int y = x + 1; y < group->r; ++y) {
            graph->dual_adjacency_matrix[x][y] = -1;
            for (int z = 0; z < group->group->v; ++z) {
                if (graph->face_matrix[z][x] && graph->face_matrix[z][y]) {
                    ++graph->dual_adjacency_matrix[x][y];
                }
            }
            graph->dual_adjacency_matrix[y][x] = graph->dual_adjacency_matrix[x][y];
        }
    }
    return 1;
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
    unsigned long long pattern = (1UL << e) - 1;
    for (int i = 0; i < adjacency_matrix_i; ++i) {
        pattern = snoob(pattern);
    }
    int max = group->v * (group->v - 1) / 2;
    if (pattern >= (1UL << max)) {
        return 0;
    }
    for (int x = 0; x < group->v; ++x) {
        for (int y = x + 1; y < group->v; ++y) {
            isogroup->adjacency_matrix[x][y] = isogroup->adjacency_matrix[y][x] = pattern & 1;
            pattern >>= 1;
        }
    }
    list_node_t list = {
        .previous = NULL,
        .next = NULL,
        .value = NULL
    };
    graph_t *graph = (graph_t *) malloc(sizeof(graph_t));
    if (!graph) {
        int tmp = errno;
        perror("malloc");
        for (int i = 0; i < group->v; ++i) {
            free(isogroup->adjacency_matrix[i]);
        }
        free(isogroup->adjacency_matrix);
        errno = tmp;
        return -1;
    }
    for (int i = 0; ; ++i) {
        int res = graph_init(graph, isogroup, i);
        if (res < 0) {
            int tmp = errno;
            perror("graph_init");
            if (list.value) {
                list_clear_and_free(&list);
            }
            for (int i = 0; i < group->v; ++i) {
                free(isogroup->adjacency_matrix[i]);
            }
            free(isogroup->adjacency_matrix);
            free(graph);
            errno = tmp;
            return -1;
        } else if (res) {
            if (list.value) {
                list_append(&list, graph);
            } else {
                list.value = graph;
            }
        } else {
            break;
        }
    }
    free(graph);
    isogroup->num_graphs = list_length(&list);
    isogroup->graphs = (graph_t *) malloc(sizeof(graph_t) * isogroup->num_graphs);
    if (!isogroup->graphs) {
        int tmp = errno;
        perror("malloc");
        list_clear_and_free(&list);
        for (int i = 0; i < group->v; ++i) {
            free(isogroup->adjacency_matrix[i]);
        }
        free(isogroup->adjacency_matrix);
        errno = tmp;
        return -1;
    }
    list_to_array(&list, isogroup->graphs, sizeof(graph_t));
    list_clear_and_free(&list);
    return 1;
}

int isomorphic_group_check_subgraphs2(isomorphic_group_t *group, char **target_adjacency_matrix, int changes) {
    if (changes > 0) {
        // Try deletions
        for (int x = 0; x < group->group->v; ++x) {
            for (int y = x; y < group->group->v; ++y) {
                if (group->adjacency_matrix[x][y]) {
                    group->adjacency_matrix[x][y] = 0;
                    group->adjacency_matrix[y][x] = 0;
                    int res = isomorphic_group_check_subgraphs2(group, target_adjacency_matrix, changes - 1);
                    group->adjacency_matrix[x][y] = 1;
                    group->adjacency_matrix[y][x] = 1;
                    if (res < 0) {
                        int tmp = errno;
                        perror("isomorphic_group_check_subgraphs2");
                        errno = tmp;
                        return -1;
                    } else if (res) {
                        return 1;
                    }
                }
            }
        }
        // TODO contractions
        return 0;
    } else {
        for (int xa = 0, xb = 0; xa < group->group->v; ++xa) {
            int do_x = 0;
            for (int y = 0; y < group->group->v; ++y) {
                if (group->adjacency_matrix[xa][y] != 0) {
                    do_x = 1;
                    break;
                }
            }
            if (do_x) {
                for (int ya = 0, yb = 0; ya < group->group->v; ++ya) {
                    int do_y = 0;
                    for (int x = 0; x < group->group->v; ++x) {
                        if (group->adjacency_matrix[x][ya] != 0) {
                            do_y = 1;
                            break;
                        }
                    }
                    if (do_y) {
                        if (group->adjacency_matrix[xa][ya] != target_adjacency_matrix[xb][yb]) {
                            return 0;
                        }
                        ++yb;
                    }
                }
                ++xb;
            }
        }
        return 1;
    }
}

int isomorphic_group_check_subgraphs(isomorphic_group_t *group, char **target_adjacency_matrix, int e) {
    int changes = group->e - e;
    if (changes > 0) {
        int res = isomorphic_group_check_subgraphs2(group, target_adjacency_matrix, changes);
        if (res < 0) {
            int tmp = errno;
            perror("isomorphic_group_check_subgraphs2");
            errno = tmp;
            return -1;
        } else {
            return res;
        }
    } else {
        return 0;
    }
}

int isomorphic_group_is_planar(isomorphic_group_t *group) {
    int res = isomorphic_group_check_subgraphs(group, (char **) K33, 9);
    if (res < 0) {
        int tmp = errno;
        perror("isomorphic_group_check_subgraphs");
        errno = tmp;
        return -1;
    } else if (res) {
        return 0;
    } else {
        res = isomorphic_group_check_subgraphs(group, (char **) K5, 10);
        if (res < 0) {
            int tmp = errno;
            perror("isomorphic_group_check_subgraphs");
            errno = tmp;
            return -1;
        } else {
            return !res;
        }
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
                free(isogroup);
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
                    free(isogroup);
                    errno = tmp;
                    return -1;
                } else if (res) {
                    if (list.value) {
                        list_append(&list, isogroup);
                    } else {
                        list.value = isogroup;
                    }
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
