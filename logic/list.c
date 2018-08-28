#include <stdlib.h>
#include <string.h>
#include <list.h>

int list_append(list_node_t *list, void *value) {
    int i;
    for (i = 0; list->next; ++i) {
        list = list->next;
    }
    list_node_t *node = (list_node_t *) malloc(sizeof(list_node_t));
    node->previous = list;
    node->next = NULL;
    node->value = value;
    list->next = node;
    return i;
}

void list_insert(list_node_t *list, void *value) {
    list_node_t *node = (list_node_t *) malloc(sizeof(list_node_t));
    node->previous = list;
    node->next = list->next;
    node->value = value;
    list->next = node;
    node->next->previous = node;
}

void list_insert2(list_node_t *list, void *value, int i) {
    for (; i > 0; --i) {
        list = list->next;
    }
    list_insert(list, value);
}

int list_length(list_node_t *list) {
    int i;
    for (i = 1; list->next; ++i) {
        list = list->next;
    }
    return i;
}

int list_total_length(list_node_t *list) {
    int i;
    for (i = list_length(list); list->next; ++i) {
        list = list->next;
    }
    return i;
}

void *list_remove(list_node_t *list) {
    list->previous->next = list->next;
    list->next->previous = list->previous;
    void *val = list->value;
    free(list);
    return val;
}

void *list_remove2(list_node_t *list, int i) {
    for (; i > 0; --i) {
        list = list->next;
    }
    return list_remove(list);
}

void list_clear(list_node_t *list) {
    list_node_t *first = list;
    if (list->next) {
        do {
            list = list->next;
            if (first != list->previous) {
                free(list->previous);
            }
        } while (list->next);
        free(list);
    }
}

void list_clear_and_free(list_node_t *list) {
    list_node_t *first = list;
    if (list->next) {
        do {
            free(list->value);
            list = list->next;
            if (first != list->previous) {
                free(list->previous);
            }
        } while (list->next);
        free(list);
        free(list->value);
    }
}

void list_to_array(list_node_t *list, void **arr) {
    while (list) {
        *arr++ = list->value;
        list = list->next;
    }
}
