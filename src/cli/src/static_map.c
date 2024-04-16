/**
 * @file    static_map.c
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-03-23
 *
 * @brief
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


#if 0
sm_node_t* sm_node_pop_back(sm_node_t** head, sm_node_t** tail) {
    if ((NULL != head) || (NULL != tail)) {
        if (NULL != *tail) {
            sm_node_t* temp = *tail;
            if (NULL != (*tail)->prev) {
                *tail = (*tail)->prev;
                (*tail)->next = NULL;
                temp->next = NULL;
                temp->prev = NULL;
            } else {
                *tail = NULL;
                *head = NULL;
            }
            return temp;
        }
    }
    return NULL;
}

void sm_node_push_front(sm_node_t** head, sm_node_t** tail, sm_node_t* item) {
    if ((NULL != head) || (NULL != tail) || (NULL != item)) {
        if (NULL == (*tail)) {
            *head = item;
            *tail = item;
            item->prev = NULL;
            item->next = NULL;
        } else {
            if (NULL == *head) {
                *head = *tail;
            }
            (*head)->prev = item;
            item->next = *tail;
            item->prev = NULL;
            *head = item;
        }
    }
}

sm_node_t* sm_node_pop_front(sm_node_t** head, sm_node_t** tail) {
    if ((NULL != head) || (NULL != tail)) {
        if (NULL != *head) {
            sm_node_t* temp = *head;
            if (NULL != (*head)->next) {
                *head = (*head)->next;
                (*head)->prev = NULL;
                temp->next = NULL;
                temp->prev = NULL;
            } else {
                *tail = NULL;
                *head = NULL;
            }
            return temp;
        }
    }
    return NULL;
}

void sm_node_insert(sm_node_t** head, sm_node_t** tail,
    sm_node_t* after, sm_node_t * item) {
    if ((NULL != head) || (NULL != tail) ||
        (NULL != after) || (NULL != item)) {
        item->prev = after;
        item->next = after->next;
        after->next = item;
        if (NULL == item->next) {
            *tail = item;
        } else {
            item->next->prev = item;
        }
    }
}

void sm_node_remove(sm_node_t** head, sm_node_t** tail, sm_node_t* item) {
    if ((NULL != head) || (NULL != tail) || (NULL != item)) {
        if (NULL == item->next) {
            sm_node_pop_back(head, tail);
        } else if (NULL == item->prev) {
            sm_node_pop_front(head, tail);
        } else {
            item->next = item->next->prev;
            item->prev = item->next->prev;
        }
    }
}

void sm_node_clear(sm_node_t** head, sm_node_t** tail) {
    if (NULL != head && NULL != tail) {
        sm_node_t* curr = *head;
        *head = NULL;
        *tail = NULL;
        while (curr) {
            sm_node_t* temp = curr;
            curr = curr->next;
            temp->prev = NULL;
            temp->next = NULL;
        }
    }
}
#endif
// typedef int(*sm_node_iterator_t)(void*, sm_node_t*);
/* Double linked list section

void sm_node_foreach(sm_node_t* head, sm_node_iterator_t callback, void* arg) {
    if (NULL != callback) {
        while (NULL != head) {
            if (callback(arg, head) == 0) {
                break;
            }
            head = head->next;
        }
    }
}

void sm_node_for(sm_node_t* head, sm_node_t* tail,
    sm_node_iterator_t callback, void* arg) {
    if ((NULL != callback) || (NULL != head)) {
        while (head != tail) {
            if (callback(arg, head) == 0) {
                break;
            }
            head = head->next;
        }
    }
}

*/


// int tools_hashmap_put(static_map_t* map, const char* key, void* value) {
//     int hash_value = sm_get_hash_key(map, key);
//     if (hash_value < 0) {
//         return -1;
//     }

//     sm_node_t* curr = map->list[hash_value].head;
//     int add_new = 1;

//     while (NULL != curr) {
//         sm_node_t* node = (sm_node_t*)curr->object;
//         if (strlen(node->key) == strlen(key)) {
//             if (0 == strcmp(node->key, key)) {
//                 node->value = value;
//                 add_new = 0;
//                 break;
//             }
//         }
//         curr = curr->next;
//     }

//     if (1 == add_new) {
//         sm_node_t* item = tools_hash_bucket_new(key, value);
//         tools_dlist_push_back(&map->list[hash_value].head,
//             &map->list[hash_value].tail, item);
//         map->count++;
//     }

//     return 0;
// }


// void tools_hashmap_remove(static_map_t* map, const char* key) {
//     int hash_value = sm_get_hash_key(map, key);
//     if (hash_value < 0) {
//         return;
//     }
//     sm_node_t* curr = map->list[hash_value].head;
//     while (NULL != curr) {
//         sm_node_t* node = (sm_node_t*)curr->object;
//         if (strlen(node->key) == strlen(key)) {
//             if (0 == strcmp(node->key, key)) {
//                 map->count--;
//                 break;
//             }
//         }
//         curr = curr->next;
//     }
//     tools_dlist_remove(&map->list[hash_value].head,
//         &map->list[hash_value].tail, curr);
//     tools_hash_bucket_delete(&curr);
// }

// void tools_hashmap_clear(static_map_t* map) {
//     if (map) {
//         for (size_t i = 0; i < map->size; i++) {
//             while (NULL != map->list[i].head) {
//                 sm_node_t* item = tools_dlist_pop_back(
//                     &map->list[i].head, &map->list[i].tail);
//                 tools_hash_bucket_delete(&item);
//             }
//         }
//         map->count = 0;
//     }
// }

// size_t tools_hashmap_count(static_map_t *map) {
//     return (NULL != map) ? map->count : 0;
// }

// size_t tools_hashmap_size(static_map_t *map) {
//     return (NULL != map) ? map->size : 0;
// }
