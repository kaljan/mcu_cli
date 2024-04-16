/**
 * @file    static_map.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-03-23
 *
 * @brief
 */
#ifndef STATIC_MAP_H
#define STATIC_MAP_H

#include <string.h>

/**
 * Static hash map node
 */
typedef struct sm_node {
    /* Pointer to key string */
    const char* key;

    /* Poniter to value or object */
    void* value;

    /* Pointer to prev list node */
    struct sm_node* prev;

    /* Pointer to next list node */
    struct sm_node* next;
} sm_node_t;

/**
 * Static hash map bucket
 */
typedef struct sm_bucket {
    /* Poniter head of bucket */
    sm_node_t* head;

    /* Poniter to tail of bucket */
    sm_node_t* tail;
} sm_bucket_t;

/**
 * Static hash map
 */
typedef struct static_map {
    /* Poniter to array of hash map bucket */
    sm_bucket_t* list;

    /* Count of bucket elements in list */
    size_t size;

    /* Count of elements in hashmap */
    size_t count;
} static_map_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int static_map_put(static_map_t* map, sm_node_t* item);
void* static_map_get(static_map_t* map, const char* key);
void static_map_foreach(static_map_t* map, int(iter_fn)(const char*,void*));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STATIC_MAP_H */
