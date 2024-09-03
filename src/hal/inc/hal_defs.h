/**
 * @file    hal_defs.h
 * @author  Mikalai Naurotski (kaljan@nothern.com)
 * @version 0.0.0
 * @date    2024-04-06
 *
 * @brief
 */
#ifndef HAL_DEFS_H
#define HAL_DEFS_H

#include <stddef.h>

#define HAL_SUCCESS                 0
#define HAL_FAILED                  -1

#define HAL_ENABLED                 1
#define HAL_DISABLED                0

#define HAL_INSTRUCTION             0
#define HAL_DATA                    1

#define HAL_DIR_WRITE               0
#define HAL_DIR_READ                1

#define HAL_FLAG_INIT_OFFSET                            ((uint32_t)0U)
#define HAL_FLAG_INIT                                   ((uint32_t)(1U << (HAL_FLAG_INIT_OFFSET)))
#define HAL_FLAG_BSY_OFFSET                             ((uint32_t)1U)
#define HAL_FLAG_BSY                                    ((uint32_t)(1U << (HAL_FLAG_BSY_OFFSET)))
#define HAL_FLAG_RDY_OFFSET                             ((uint32_t)2U)
#define HAL_FLAG_RDY                                    ((uint32_t)(1U << (HAL_FLAG_RDY_OFFSET)))

#define HAL_WRITE_REG(reg, value)                       (reg) = (value)
#define HAL_READ_REG(reg)                               (reg)
#define HAL_READ_BITS(reg, mask)                        (reg) & (mask)
#define HAL_SET_BITS(reg, mask)                         (reg) |= (mask)
#define HAL_CLEAR_BITS(reg, mask)                       (reg) &= ~(mask)
#define HAL_SET_BIT(reg, offset)                        HAL_SET_BITS(reg, 1U << (offset))
#define HAL_CLEAR_BIT(reg, offset)                      HAL_CLEAR_BITS(reg, 1U << (offset))
#define HAL_MODIFY_REG(reg, value, mask)                (reg) = ((reg) & (~(mask))) | ((value) & (mask))
#define HAL_MODIFY_FIELD(reg, value, mask, offset)      (reg) = ((reg) & (~((mask) << (offset)))) | (((value) & (mask)) << (offset))
#define HAL_IS_BIT_SET(reg, mask)                       ((HAL_READ_BITS(reg, mask)) == (mask))
#define HAL_IS_BIT_CLEAR(reg, mask)                     ((HAL_READ_BITS(reg, mask)) != (mask))
#define HAL_COMPARE_BITS(reg, value, mask)              ((HAL_READ_BITS(reg, mask)) == (HAL_READ_BITS(value, mask)))
#define HAL_READ_FIELD(reg, mask, offset)               (((reg) & ((mask) << (offset))) >> (offset))
#define HAL_COMPARE_FIELD(reg, value, mask, offset)     (((reg) & ((mask) << (offset))) == (((value) & (mask)) << (offset)))
#define HAL_FIELD_EQ(reg, value, mask, offset)          (((reg) & ((mask) << (offset))) == (((value) & (mask)) << (offset)))
#define HAL_FIELD_NE(reg, value, mask, offset)          (((reg) & ((mask) << (offset))) != (((value) & (mask)) << (offset)))

#define HAL_ENABLE_BIT(reg, mask, enable) \
    if (0 == (enable)) { \
        HAL_CLEAR_BITS(reg, mask); \
    } else { \
        HAL_SET_BITS(reg, mask); \
    }

#define HAL_ASSERT_PTR(ptr) \
    if (ptr == NULL) { \
        return; \
    }

#define HAL_ASSERT_PTR_RET(ptr, ret) \
    if (ptr == NULL) { \
        return ret; \
    }

#define HAL_ASSERT_OBJECT(type, name, ptr) \
    type* name = (type*)ptr; \
    if (ptr == NULL) { \
        return; \
    }

#define HAL_ASSERT_OBJECT_RET(type, name, ptr, ret) \
    type* name = (type*)ptr; \
    if (ptr == NULL) { \
        return; \
    }

#define HAL_CONEXT(type, arg) type* context = (type*)(arg)

#define HAL_OBJECT(type, name, arg) type* name = (type*)(arg)

#define HAL_U16_LOW_BYTE(u16_val)   (0x00FF & (u16_val))
#define HAL_U16_HIGH_BYTE(u16_val)   ((0xFF00 & (u16_val)) >> 8)

#define HAL_ALIGN(a)    __attribute__ ((aligned (a)))

#endif /* HAL_DEFS_H */
