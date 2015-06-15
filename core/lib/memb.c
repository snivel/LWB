/*
 * Copyright (c) 2004, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/**
 * \addtogroup memb
 * @{
 */

 /**
 * \file
 * Memory block allocation routines.
 * \author Adam Dunkels <adam@sics.se>
 */
#include <string.h>

#include "contiki.h"
#include "lib/memb.h"

/*---------------------------------------------------------------------------*/
void
memb_init(struct memb *m)
{
  memset(m->count, 0, (m->num + 7) >> 3);
  memset(m->mem, 0, m->size * m->num);
}
/*---------------------------------------------------------------------------*/
void
membx_init(struct membx *m)
{
  memset(m->count, 0, (m->num + 7) >> 3);
  m->mem = fram_alloc(m->size * m->num);
  if (FRAM_ALLOC_ERROR == m->mem) {
    DEBUG_PRINT_ERROR("memory allocation failed!");
  }
}
/*---------------------------------------------------------------------------*/
void *
memb_alloc(struct memb *m)
{
  unsigned short i, limit;
  uint8_t bit;
 
  i = m->last;
  limit = m->num;
find_free:
  for (; i < limit; i++) {      // loop through all data units in this memory block
    bit = (1 << (i & 0x07));
    if (0 == (m->count[i >> 3] & bit)) {     // bit set?
      /* If this block was unused, we increase the reference count to
	     indicate that it now is used and return a pointer to the
	     memory block. */
      m->count[i >> 3] |= bit;    // set bit
      m->last = i;
      return (void *)((char *)m->mem + (i * (unsigned short)m->size));
    }
  }
  if (m->last != 0 && i == m->num) {
    i = 0;
    limit = m->last;
    goto find_free;
  }  
  /* No free block was found, so we return NULL to indicate failure to
     allocate block. */
  m->last = 0;
  return NULL;
}
/*---------------------------------------------------------------------------*/
uint32_t
membx_alloc(struct membx *m)
{
  unsigned short i, limit;
  uint8_t bit;
 
  i = m->last;
  limit = m->num;
find_free:
  for (; i < limit; i++) {      // loop through all data units in this memory block
    bit = (1 << (i & 0x07));
    if (0 == (m->count[i >> 3] & bit)) {     // bit set?
      m->count[i >> 3] |= bit;    // set bit
      m->last = i;
      m->n_alloc++;
      return m->mem + ((uint32_t)i * (uint32_t)m->size);    // return the address
    }
  }
  if (m->last != 0 && i == m->num) {
    i = 0;
    limit = m->last;
    goto find_free;
  }  
  /* No free block was found, so we return XMEM_INVALID_ADDR to indicate failure to
     allocate block. */
  m->last = 0;
  return XMEM_INVALID_ADDR;
}
/*---------------------------------------------------------------------------*/
void
memb_free(struct memb *m, void *ptr)
{
  unsigned short i = (ptr - m->mem) / m->size;
  if (i < m->num) {
      m->count[i >> 3] &= ~(1 << (i & 0x07));    // clear bit
  }
}
/*---------------------------------------------------------------------------*/
void
membx_free(struct membx *m, uint32_t ptr)
{
  unsigned short i = (ptr - m->mem) / m->size;
  if ( (i < m->num) && (m->count[i >> 3] & (1 << (i & 0x07))) ) {
    m->count[i >> 3] &= ~(1 << (i & 0x07));    // clear bit
    m->n_alloc--;
  }
}
/*---------------------------------------------------------------------------*/
uint32_t
membx_get_next(struct membx *m, uint16_t start_idx)      // returns the first non-empty block
{
  uint16_t i;
  uint8_t bit;
  if (start_idx >= m->num) {
    start_idx = 0;
  }
  i = start_idx;
  do {                         // loop through all data units in this memory block
    bit = (1 << (i & 0x07));
    if (m->count[i >> 3] & bit) {     // bit set?
      return m->mem + ((uint32_t)i * (uint32_t)m->size);    // return the address
    }
    i++;
    if (i == m->num) {
        i = 0;
    }
  } while (i != start_idx);
  return XMEM_INVALID_ADDR;
}
/*---------------------------------------------------------------------------*/
int
memb_inmemb(struct memb *m, void *ptr)
{
  return (char *)ptr >= (char *)m->mem &&
    (char *)ptr < (char *)m->mem + (m->num * m->size);
}
/*---------------------------------------------------------------------------*/

/** @} */