/**
 * @file    drivers_rcu.h
 * @author  kaljan
 * @version 0.0.3
 * @date    Aug 16, 2020
 *
 * @brief
 */

#ifndef DRIVERS_RCU_H
#define DRIVERS_RCU_H

#include "key_codes.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*RCU_EventCallback_t)(void*, uint32_t);

int RCU_KeyToDigit(uint32_t key);
void RCU_SetCallback(RCU_EventCallback_t callback, void* context);
void RCU_Initialize(void);
void RCU_Update(void);

#endif /* DRIVERS_RCU_H */
