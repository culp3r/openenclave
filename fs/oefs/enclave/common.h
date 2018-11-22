// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef _OE_OEFS_COMMON_H
#define _OE_OEFS_COMMON_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>
#include <openenclave/internal/defs.h>
#include <openenclave/internal/atomic.h>
#include <string.h>
#include <stdlib.h>

#define OEFS_PATH_MAX 256
#define OEFS_KEY_SIZE 32
#define OEFS_BLOCK_SIZE 1024

typedef struct _oe_blk
{
    uint8_t data[OEFS_BLOCK_SIZE];
} oe_blk_t;

#endif /* _OE_OEFS_COMMON_H */