#ifndef LIST_H
#define LIST_H

struct list_node;
typedef struct list_node list_node_t;

struct list_node {
    list_node_t *previous;
    list_node_t *next;
    void *value;
};

int list_append(list_node_t *list, void *value);
void list_insert(list_node_t *list, void *value);
void list_insert2(list_node_t *list, void *value, int i);
int list_length(list_node_t *list);
int list_total_length(list_node_t *list);
void *list_remove(list_node_t *list);
void *list_remove2(list_node_t *list, int i);
void list_clear(list_node_t *list);
void list_clear_and_free(list_node_t *list);
void list_to_array(list_node_t *list, void **arr);

#endif
