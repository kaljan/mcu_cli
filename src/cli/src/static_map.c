/**
 * @file    static_map.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-03-23
 *
 * @brief   Static hash map for CLI
 */

#include "static_map.h"

void sm_node_push_back(sm_node_t** head, sm_node_t** tail, sm_node_t* item) {
    if ((NULL != head) || (NULL != tail) || (NULL != item)) {
        if (NULL == (*head)) {
            *head = item;
            *tail = item;
            item->prev = NULL;
            item->next = NULL;
        } else {
            if (NULL == *tail) {
                *tail = *head;
            }
            (*tail)->next = item;
            item->prev = *tail;
            item->next = NULL;
            *tail = item;
        }
    }
}

static int sm_hash_func(const char* key, size_t map_size) {
    int ret = -1;
    if ((NULL != key && (strlen(key) > 0))) {
        size_t temp = 0;
        while (*key != 0) {
            temp += *key;
            key++;
        }
        temp = temp % map_size;
        ret = temp;
    }
    return ret;
}

static int sm_get_hash_key(static_map_t* map, const char* key) {
    int hash_value = -1;
    if (NULL != map) {
        hash_value = sm_hash_func(key, map->size);
        if (hash_value > map->size) {
            hash_value = -1;
        }
    }
    return hash_value;
}

int static_map_put(static_map_t* map, sm_node_t* item) {
    if (item != NULL) {
        int hash_value = sm_get_hash_key(map, item->key);
        if (hash_value < 0) {
            return -1;
        }

        sm_node_t* curr = map->list[hash_value].head;
        int add_new = 1;

        while (NULL != curr) {
            if (strlen(item->key) == strlen(curr->key)) {
                if (0 == strcmp(item->key, curr->key)) {
                    add_new = 0;
                    break;
                }
            }
            curr = curr->next;
        }

        if (1 == add_new) {
            sm_node_push_back(&map->list[hash_value].head,
                &map->list[hash_value].tail, item);
            map->count++;
        }
        return 0;
    }
    return -1;
}

void* static_map_get(static_map_t* map, const char* key) {
    void* value = NULL;
    int hash_value = sm_get_hash_key(map, key);

    if (hash_value < 0) {
        return NULL;
    }

    sm_node_t* curr = map->list[hash_value].head;
    while (NULL != curr) {
        if (strlen(curr->key) == strlen(key)) {
            if (0 == strcmp(curr->key, key)) {
                value = curr->value;
                break;
            }
        }
        curr = curr->next;
    }
    return value;
}

void static_map_foreach(static_map_t* map, int(iter_fn)(const char*,void*)) {
    if ((NULL != map) && (NULL != iter_fn)) {
        for (size_t i = 0; i < map->size; i++) {
            sm_node_t* node = map->list[i].head;
            while (NULL != node) {
                if (iter_fn(node->key, node->value) != 0) {
                    return;
                }
                node = node->next;
            }
        }
    }
}
