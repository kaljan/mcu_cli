/**
 * @file    hal_types.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-06
 *
 * @brief
 */
#ifndef HAL_TYPES_H
#define HAL_TYPES_H

#include <stdint.h>

typedef void* hal_object_t;
typedef void* hal_ptr_t;

typedef void (*hal_callback_t)(void*);
typedef void (*hal_on_failed_t)(void*, int);

#endif /* HAL_TYPES_H */
