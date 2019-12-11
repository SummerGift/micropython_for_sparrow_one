/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Date           Author       Notes
 * 2019-07-27     liu2guang    first implementation
 */

#ifndef __WAVUTILS_H__
#define __WAVUTILS_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t  beu08get(void *p);
uint16_t beu16get(void *p);
uint32_t beu24get(void *p);
uint32_t beu32get(void *p);
uint64_t beu64get(void *p);

int8_t  bes08get(void *p);
int16_t bes16get(void *p);
int32_t bes24get(void *p);
int32_t bes32get(void *p);
int64_t bes64get(void *p);

uint8_t be08put(void *p, uint8_t  value);
uint8_t be16put(void *p, uint16_t value);
uint8_t be24put(void *p, uint32_t value);
uint8_t be32put(void *p, uint32_t value);
uint8_t be64put(void *p, uint64_t value);

uint8_t  leu08get(void *p);
uint16_t leu16get(void *p);
uint32_t leu24get(void *p);
uint32_t leu32get(void *p);
uint64_t leu64get(void *p);

int8_t  les08get(void *p);
int16_t les16get(void *p);
int32_t les24get(void *p);
int32_t les32get(void *p);
int64_t les64get(void *p);

uint8_t le08put(void *p, uint8_t  value);
uint8_t le16put(void *p, uint16_t value);
uint8_t le24put(void *p, uint32_t value);
uint8_t le32put(void *p, uint32_t value);
uint8_t le64put(void *p, uint64_t value);

#endif
