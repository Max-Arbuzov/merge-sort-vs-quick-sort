/* Copyright (c) 2010-2024 Christopher Swenson. */
/* Copyright (c) 2012 Vojtech Fried. */
/* Copyright (c) 2012 Google Inc. All Rights Reserved. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifndef SORT_NAME
#error "Must declare SORT_NAME"
#endif

#ifndef SORT_TYPE
#error "Must declare SORT_TYPE"
#endif

#ifndef SORT_CMP
#define SORT_CMP(x, y)  ((x) < (y) ? -1 : ((y) < (x) ? 1 : 0))
#endif

#ifndef SORT_DEF
#define SORT_DEF
#else
#ifdef __GNUC__
#define SORT_GCC_PUSH
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#endif

#ifdef __cplusplus
#ifndef SORT_SAFE_CPY
#define SORT_SAFE_CPY 0
#endif
#else
#undef SORT_SAFE_CPY
#define SORT_SAFE_CPY 0
#endif

#ifndef SORT_SWAP
#define SORT_SWAP(x,y) {SORT_TYPE _sort_swap_temp = (x); (x) = (y); (y) = _sort_swap_temp;}
#endif

/* Common, type-agnostic functions and constants that we don't want to declare twice. */
#ifndef SORT_COMMON_H
#define SORT_COMMON_H

#ifndef MAX
#define MAX(x,y) (((x) > (y) ? (x) : (y)))
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y) ? (x) : (y)))
#endif

#ifndef CLZ
/* clang-only */
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif
#if __has_builtin(__builtin_clzll) || (defined(__GNUC__) && ((__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ > 3)))
#define CLZ __builtin_clzll
#else

static int clzll(uint64_t);

/* adapted from Hacker's Delight */
static int clzll(uint64_t x) {
  int n;

  if (x == 0) {
    return 64;
  }

  n = 0;

  if (x <= 0x00000000FFFFFFFFL) {
    n = n + 32;
    x = x << 32;
  }

  if (x <= 0x0000FFFFFFFFFFFFL) {
    n = n + 16;
    x = x << 16;
  }

  if (x <= 0x00FFFFFFFFFFFFFFL) {
    n = n + 8;
    x = x << 8;
  }

  if (x <= 0x0FFFFFFFFFFFFFFFL) {
    n = n + 4;
    x = x << 4;
  }

  if (x <= 0x3FFFFFFFFFFFFFFFL) {
    n = n + 2;
    x = x << 2;
  }

  if (x <= 0x7FFFFFFFFFFFFFFFL) {
    n = n + 1;
  }

  return n;
}

#define CLZ clzll
#endif
#endif

#endif /* SORT_COMMON_H */

#define SORT_CONCAT(x, y) x ## _ ## y
#define SORT_MAKE_STR1(x, y) SORT_CONCAT(x,y)
#define SORT_MAKE_STR(x) SORT_MAKE_STR1(SORT_NAME,x)

#ifndef SMALL_SORT_BND
#define SMALL_SORT_BND 16
#endif
#ifndef SMALL_SORT
#define SMALL_SORT BITONIC_SORT
#endif
#ifndef SMALL_STABLE_SORT
#define SMALL_STABLE_SORT BINARY_INSERTION_SORT
#endif

#define SORT_TYPE_CPY                  SORT_MAKE_STR(sort_type_cpy)
#define SORT_TYPE_MOVE                 SORT_MAKE_STR(sort_type_move)
#define SORT_NEW_BUFFER                SORT_MAKE_STR(sort_new_buffer)
#define SORT_DELETE_BUFFER             SORT_MAKE_STR(sort_delete_buffer)
#define BITONIC_SORT                   SORT_MAKE_STR(bitonic_sort)
#define BINARY_INSERTION_FIND          SORT_MAKE_STR(binary_insertion_find)
#define BINARY_INSERTION_SORT_START    SORT_MAKE_STR(binary_insertion_sort_start)
#define BINARY_INSERTION_SORT          SORT_MAKE_STR(binary_insertion_sort)
#define REVERSE_ELEMENTS               SORT_MAKE_STR(reverse_elements)
#define HEAP_SORT                      SORT_MAKE_STR(heap_sort)
#define MEDIAN                         SORT_MAKE_STR(median)
#define QUICK_SORT                     SORT_MAKE_STR(quick_sort)
#define MERGE_SORT                     SORT_MAKE_STR(merge_sort)
#define MERGE_SORT_RECURSIVE           SORT_MAKE_STR(merge_sort_recursive)
#define QUICK_SORT_PARTITION           SORT_MAKE_STR(quick_sort_partition)
#define QUICK_SORT_RECURSIVE           SORT_MAKE_STR(quick_sort_recursive)
#define HEAP_SIFT_DOWN                 SORT_MAKE_STR(heap_sift_down)
#define HEAPIFY                        SORT_MAKE_STR(heapify)

#ifndef MAX
#define MAX(x,y) (((x) > (y) ? (x) : (y)))
#endif
#ifndef MIN
#define MIN(x,y) (((x) < (y) ? (x) : (y)))
#endif
#ifndef SORT_CSWAP
#define SORT_CSWAP(x, y) { if(SORT_CMP((x),(y)) > 0) {SORT_SWAP((x),(y));}}
#endif

SORT_DEF void BINARY_INSERTION_SORT(SORT_TYPE *dst, const size_t size);
SORT_DEF void HEAP_SORT(SORT_TYPE *dst, const size_t size);
SORT_DEF void QUICK_SORT(SORT_TYPE *dst, const size_t size);
SORT_DEF void MERGE_SORT(SORT_TYPE *dst, const size_t size);
SORT_DEF void BITONIC_SORT(SORT_TYPE *dst, const size_t size);

/* The full implementation of a bitonic sort is not here. Since we only want to use
   sorting networks for small length lists we create optimal sorting networks for
   lists of length <= 16 and call out to BINARY_INSERTION_SORT for anything larger
   than 16.
   Optimal sorting networks for small length lists.
   Taken from https://pages.ripco.net/~jgamble/nw.html */
#define BITONIC_SORT_2          SORT_MAKE_STR(bitonic_sort_2)
static __inline void BITONIC_SORT_2(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
}


#define BITONIC_SORT_3          SORT_MAKE_STR(bitonic_sort_3)
static __inline void BITONIC_SORT_3(SORT_TYPE *dst) {
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[0], dst[1]);
}


#define BITONIC_SORT_4          SORT_MAKE_STR(bitonic_sort_4)
static __inline void BITONIC_SORT_4(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[1], dst[2]);
}


#define BITONIC_SORT_5          SORT_MAKE_STR(bitonic_sort_5)
static __inline void BITONIC_SORT_5(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[0], dst[3]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[1], dst[2]);
}


#define BITONIC_SORT_6          SORT_MAKE_STR(bitonic_sort_6)
static __inline void BITONIC_SORT_6(SORT_TYPE *dst) {
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[2], dst[5]);
  SORT_CSWAP(dst[0], dst[3]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[2], dst[3]);
}


#define BITONIC_SORT_7          SORT_MAKE_STR(bitonic_sort_7)
static __inline void BITONIC_SORT_7(SORT_TYPE *dst) {
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[0], dst[4]);
  SORT_CSWAP(dst[1], dst[5]);
  SORT_CSWAP(dst[0], dst[3]);
  SORT_CSWAP(dst[2], dst[5]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[2], dst[3]);
}


#define BITONIC_SORT_8          SORT_MAKE_STR(bitonic_sort_8)
static __inline void BITONIC_SORT_8(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[5], dst[7]);
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[0], dst[4]);
  SORT_CSWAP(dst[3], dst[7]);
  SORT_CSWAP(dst[1], dst[5]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[3], dst[6]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[3], dst[4]);
}


#define BITONIC_SORT_9          SORT_MAKE_STR(bitonic_sort_9)
static __inline void BITONIC_SORT_9(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[7], dst[8]);
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[2], dst[5]);
  SORT_CSWAP(dst[0], dst[3]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[5], dst[8]);
  SORT_CSWAP(dst[3], dst[6]);
  SORT_CSWAP(dst[4], dst[7]);
  SORT_CSWAP(dst[2], dst[5]);
  SORT_CSWAP(dst[0], dst[3]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[5], dst[7]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[2], dst[3]);
}


#define BITONIC_SORT_10          SORT_MAKE_STR(bitonic_sort_10)
static __inline void BITONIC_SORT_10(SORT_TYPE *dst) {
  SORT_CSWAP(dst[4], dst[9]);
  SORT_CSWAP(dst[3], dst[8]);
  SORT_CSWAP(dst[2], dst[7]);
  SORT_CSWAP(dst[1], dst[6]);
  SORT_CSWAP(dst[0], dst[5]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[6], dst[9]);
  SORT_CSWAP(dst[0], dst[3]);
  SORT_CSWAP(dst[5], dst[8]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[3], dst[6]);
  SORT_CSWAP(dst[7], dst[9]);
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[5], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[7], dst[8]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[2], dst[5]);
  SORT_CSWAP(dst[6], dst[8]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[4], dst[7]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[4], dst[5]);
}


#define BITONIC_SORT_11          SORT_MAKE_STR(bitonic_sort_11)
static __inline void BITONIC_SORT_11(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[5], dst[7]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[8], dst[10]);
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[9], dst[10]);
  SORT_CSWAP(dst[0], dst[4]);
  SORT_CSWAP(dst[3], dst[7]);
  SORT_CSWAP(dst[1], dst[5]);
  SORT_CSWAP(dst[6], dst[10]);
  SORT_CSWAP(dst[4], dst[8]);
  SORT_CSWAP(dst[5], dst[9]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[0], dst[4]);
  SORT_CSWAP(dst[3], dst[8]);
  SORT_CSWAP(dst[1], dst[5]);
  SORT_CSWAP(dst[6], dst[10]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[7], dst[10]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[6], dst[8]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[7], dst[9]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[7], dst[8]);
}


#define BITONIC_SORT_12          SORT_MAKE_STR(bitonic_sort_12)
static __inline void BITONIC_SORT_12(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[10], dst[11]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[5], dst[7]);
  SORT_CSWAP(dst[9], dst[11]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[8], dst[10]);
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[9], dst[10]);
  SORT_CSWAP(dst[0], dst[4]);
  SORT_CSWAP(dst[7], dst[11]);
  SORT_CSWAP(dst[1], dst[5]);
  SORT_CSWAP(dst[6], dst[10]);
  SORT_CSWAP(dst[3], dst[7]);
  SORT_CSWAP(dst[4], dst[8]);
  SORT_CSWAP(dst[5], dst[9]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[0], dst[4]);
  SORT_CSWAP(dst[7], dst[11]);
  SORT_CSWAP(dst[3], dst[8]);
  SORT_CSWAP(dst[1], dst[5]);
  SORT_CSWAP(dst[6], dst[10]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[7], dst[10]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[6], dst[8]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[7], dst[9]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[7], dst[8]);
}


#define BITONIC_SORT_13          SORT_MAKE_STR(bitonic_sort_13)
static __inline void BITONIC_SORT_13(SORT_TYPE *dst) {
  SORT_CSWAP(dst[1], dst[7]);
  SORT_CSWAP(dst[9], dst[11]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[5], dst[8]);
  SORT_CSWAP(dst[0], dst[12]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[8], dst[11]);
  SORT_CSWAP(dst[7], dst[12]);
  SORT_CSWAP(dst[5], dst[9]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[3], dst[7]);
  SORT_CSWAP(dst[10], dst[11]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[6], dst[12]);
  SORT_CSWAP(dst[7], dst[8]);
  SORT_CSWAP(dst[11], dst[12]);
  SORT_CSWAP(dst[4], dst[9]);
  SORT_CSWAP(dst[6], dst[10]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[10], dst[11]);
  SORT_CSWAP(dst[1], dst[7]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[9], dst[11]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[4], dst[7]);
  SORT_CSWAP(dst[8], dst[10]);
  SORT_CSWAP(dst[0], dst[5]);
  SORT_CSWAP(dst[2], dst[5]);
  SORT_CSWAP(dst[6], dst[8]);
  SORT_CSWAP(dst[9], dst[10]);
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[7], dst[8]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[5], dst[6]);
}


#define BITONIC_SORT_14          SORT_MAKE_STR(bitonic_sort_14)
static __inline void BITONIC_SORT_14(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[10], dst[11]);
  SORT_CSWAP(dst[12], dst[13]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[8], dst[10]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[5], dst[7]);
  SORT_CSWAP(dst[9], dst[11]);
  SORT_CSWAP(dst[0], dst[4]);
  SORT_CSWAP(dst[8], dst[12]);
  SORT_CSWAP(dst[1], dst[5]);
  SORT_CSWAP(dst[9], dst[13]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[3], dst[7]);
  SORT_CSWAP(dst[0], dst[8]);
  SORT_CSWAP(dst[1], dst[9]);
  SORT_CSWAP(dst[2], dst[10]);
  SORT_CSWAP(dst[3], dst[11]);
  SORT_CSWAP(dst[4], dst[12]);
  SORT_CSWAP(dst[5], dst[13]);
  SORT_CSWAP(dst[5], dst[10]);
  SORT_CSWAP(dst[6], dst[9]);
  SORT_CSWAP(dst[3], dst[12]);
  SORT_CSWAP(dst[7], dst[11]);
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[4], dst[8]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[7], dst[13]);
  SORT_CSWAP(dst[2], dst[8]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[9], dst[10]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[11], dst[13]);
  SORT_CSWAP(dst[3], dst[8]);
  SORT_CSWAP(dst[7], dst[12]);
  SORT_CSWAP(dst[6], dst[8]);
  SORT_CSWAP(dst[10], dst[12]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[7], dst[9]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[7], dst[8]);
  SORT_CSWAP(dst[9], dst[10]);
  SORT_CSWAP(dst[11], dst[12]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
}


#define BITONIC_SORT_15          SORT_MAKE_STR(bitonic_sort_15)
static __inline void BITONIC_SORT_15(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[10], dst[11]);
  SORT_CSWAP(dst[12], dst[13]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[8], dst[10]);
  SORT_CSWAP(dst[12], dst[14]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[5], dst[7]);
  SORT_CSWAP(dst[9], dst[11]);
  SORT_CSWAP(dst[0], dst[4]);
  SORT_CSWAP(dst[8], dst[12]);
  SORT_CSWAP(dst[1], dst[5]);
  SORT_CSWAP(dst[9], dst[13]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[10], dst[14]);
  SORT_CSWAP(dst[3], dst[7]);
  SORT_CSWAP(dst[0], dst[8]);
  SORT_CSWAP(dst[1], dst[9]);
  SORT_CSWAP(dst[2], dst[10]);
  SORT_CSWAP(dst[3], dst[11]);
  SORT_CSWAP(dst[4], dst[12]);
  SORT_CSWAP(dst[5], dst[13]);
  SORT_CSWAP(dst[6], dst[14]);
  SORT_CSWAP(dst[5], dst[10]);
  SORT_CSWAP(dst[6], dst[9]);
  SORT_CSWAP(dst[3], dst[12]);
  SORT_CSWAP(dst[13], dst[14]);
  SORT_CSWAP(dst[7], dst[11]);
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[4], dst[8]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[7], dst[13]);
  SORT_CSWAP(dst[2], dst[8]);
  SORT_CSWAP(dst[11], dst[14]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[9], dst[10]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[11], dst[13]);
  SORT_CSWAP(dst[3], dst[8]);
  SORT_CSWAP(dst[7], dst[12]);
  SORT_CSWAP(dst[6], dst[8]);
  SORT_CSWAP(dst[10], dst[12]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[7], dst[9]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[7], dst[8]);
  SORT_CSWAP(dst[9], dst[10]);
  SORT_CSWAP(dst[11], dst[12]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
}


#define BITONIC_SORT_16          SORT_MAKE_STR(bitonic_sort_16)
static __inline void BITONIC_SORT_16(SORT_TYPE *dst) {
  SORT_CSWAP(dst[0], dst[1]);
  SORT_CSWAP(dst[2], dst[3]);
  SORT_CSWAP(dst[4], dst[5]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
  SORT_CSWAP(dst[10], dst[11]);
  SORT_CSWAP(dst[12], dst[13]);
  SORT_CSWAP(dst[14], dst[15]);
  SORT_CSWAP(dst[0], dst[2]);
  SORT_CSWAP(dst[4], dst[6]);
  SORT_CSWAP(dst[8], dst[10]);
  SORT_CSWAP(dst[12], dst[14]);
  SORT_CSWAP(dst[1], dst[3]);
  SORT_CSWAP(dst[5], dst[7]);
  SORT_CSWAP(dst[9], dst[11]);
  SORT_CSWAP(dst[13], dst[15]);
  SORT_CSWAP(dst[0], dst[4]);
  SORT_CSWAP(dst[8], dst[12]);
  SORT_CSWAP(dst[1], dst[5]);
  SORT_CSWAP(dst[9], dst[13]);
  SORT_CSWAP(dst[2], dst[6]);
  SORT_CSWAP(dst[10], dst[14]);
  SORT_CSWAP(dst[3], dst[7]);
  SORT_CSWAP(dst[11], dst[15]);
  SORT_CSWAP(dst[0], dst[8]);
  SORT_CSWAP(dst[1], dst[9]);
  SORT_CSWAP(dst[2], dst[10]);
  SORT_CSWAP(dst[3], dst[11]);
  SORT_CSWAP(dst[4], dst[12]);
  SORT_CSWAP(dst[5], dst[13]);
  SORT_CSWAP(dst[6], dst[14]);
  SORT_CSWAP(dst[7], dst[15]);
  SORT_CSWAP(dst[5], dst[10]);
  SORT_CSWAP(dst[6], dst[9]);
  SORT_CSWAP(dst[3], dst[12]);
  SORT_CSWAP(dst[13], dst[14]);
  SORT_CSWAP(dst[7], dst[11]);
  SORT_CSWAP(dst[1], dst[2]);
  SORT_CSWAP(dst[4], dst[8]);
  SORT_CSWAP(dst[1], dst[4]);
  SORT_CSWAP(dst[7], dst[13]);
  SORT_CSWAP(dst[2], dst[8]);
  SORT_CSWAP(dst[11], dst[14]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[9], dst[10]);
  SORT_CSWAP(dst[2], dst[4]);
  SORT_CSWAP(dst[11], dst[13]);
  SORT_CSWAP(dst[3], dst[8]);
  SORT_CSWAP(dst[7], dst[12]);
  SORT_CSWAP(dst[6], dst[8]);
  SORT_CSWAP(dst[10], dst[12]);
  SORT_CSWAP(dst[3], dst[5]);
  SORT_CSWAP(dst[7], dst[9]);
  SORT_CSWAP(dst[3], dst[4]);
  SORT_CSWAP(dst[5], dst[6]);
  SORT_CSWAP(dst[7], dst[8]);
  SORT_CSWAP(dst[9], dst[10]);
  SORT_CSWAP(dst[11], dst[12]);
  SORT_CSWAP(dst[6], dst[7]);
  SORT_CSWAP(dst[8], dst[9]);
}

SORT_DEF void BITONIC_SORT(SORT_TYPE *dst, const size_t size) {
  switch (size) {
  case 0:
  case 1:
    break;

  case 2:
    BITONIC_SORT_2(dst);
    break;

  case 3:
    BITONIC_SORT_3(dst);
    break;

  case 4:
    BITONIC_SORT_4(dst);
    break;

  case 5:
    BITONIC_SORT_5(dst);
    break;

  case 6:
    BITONIC_SORT_6(dst);
    break;

  case 7:
    BITONIC_SORT_7(dst);
    break;

  case 8:
    BITONIC_SORT_8(dst);
    break;

  case 9:
    BITONIC_SORT_9(dst);
    break;

  case 10:
    BITONIC_SORT_10(dst);
    break;

  case 11:
    BITONIC_SORT_11(dst);
    break;

  case 12:
    BITONIC_SORT_12(dst);
    break;

  case 13:
    BITONIC_SORT_13(dst);
    break;

  case 14:
    BITONIC_SORT_14(dst);
    break;

  case 15:
    BITONIC_SORT_15(dst);
    break;

  case 16:
    BITONIC_SORT_16(dst);
    break;

  default:
    BINARY_INSERTION_SORT(dst, size);
  }
}

#if SORT_SAFE_CPY

SORT_DEF void SORT_TYPE_CPY(SORT_TYPE *dst, SORT_TYPE *src, const size_t size) {
  size_t i = 0;

  for (; i < size; ++i) {
    dst[i] = src[i];
  }
}

SORT_DEF void SORT_TYPE_MOVE(SORT_TYPE *dst, SORT_TYPE *src, const size_t size) {
  size_t i;

  if (dst < src) {
    SORT_TYPE_CPY(dst, src, size);
  } else if (dst != src && size > 0) {
    for (i = size - 1; i > 0; --i) {
      dst[i] = src[i];
    }

    *dst = *src;
  }
}

#else

#undef SORT_TYPE_CPY
#define SORT_TYPE_CPY(dst, src, size) memcpy((dst), (src), (size) * sizeof(SORT_TYPE))
#undef SORT_TYPE_MOVE
#define SORT_TYPE_MOVE(dst, src, size) memmove((dst), (src), (size) * sizeof(SORT_TYPE))

#endif

SORT_DEF SORT_TYPE* SORT_NEW_BUFFER(size_t size) {
#if SORT_SAFE_CPY
  return new SORT_TYPE[size];
#else
  return (SORT_TYPE*)malloc(size * sizeof(SORT_TYPE));
#endif
}

SORT_DEF void SORT_DELETE_BUFFER(SORT_TYPE* pointer) {
#if SORT_SAFE_CPY
  delete[] pointer;
#else
  free(pointer);
#endif
}

/* Function used to do a binary search for binary insertion sort */
static __inline size_t BINARY_INSERTION_FIND(SORT_TYPE *dst, const SORT_TYPE x,
    const size_t size) {
  size_t l, c, r;
  SORT_TYPE cx;
  l = 0;
  r = size - 1;
  c = r >> 1;

  /* check for out of bounds at the beginning. */
  if (SORT_CMP(x, dst[0]) < 0) {
    return 0;
  } else if (SORT_CMP(x, dst[r]) > 0) {
    return r;
  }

  cx = dst[c];

  while (1) {
    const int val = SORT_CMP(x, cx);

    if (val < 0) {
      if (c - l <= 1) {
        return c;
      }

      r = c;
    } else { /* allow = for stability. The binary search favors the right. */
      if (r - c <= 1) {
        return c + 1;
      }

      l = c;
    }

    c = l + ((r - l) >> 1);
    cx = dst[c];
  }
}

/* Binary insertion sort, but knowing that the first "start" entries are sorted.  Used in timsort. */
static void BINARY_INSERTION_SORT_START(SORT_TYPE *dst, const size_t start, const size_t size) {
  size_t i;

  for (i = start; i < size; i++) {
    size_t j;
    SORT_TYPE x;
    size_t location;

    /* If this entry is already correct, just move along */
    if (SORT_CMP(dst[i - 1], dst[i]) <= 0) {
      continue;
    }

    /* Else we need to find the right place, shift everything over, and squeeze in */
    x = dst[i];
    location = BINARY_INSERTION_FIND(dst, x, i);

    for (j = i - 1; j >= location; j--) {
      dst[j + 1] = dst[j];

      if (j == 0) { /* check edge case because j is unsigned */
        break;
      }
    }

    dst[location] = x;
  }
}

/* Binary insertion sort */
SORT_DEF void BINARY_INSERTION_SORT(SORT_TYPE *dst, const size_t size) {
  /* don't bother sorting an array of size <= 1 */
  if (size <= 1) {
    return;
  }

  BINARY_INSERTION_SORT_START(dst, 1, size);
}

/* Standard merge sort */
SORT_DEF void MERGE_SORT_RECURSIVE(SORT_TYPE *newdst, SORT_TYPE *dst, const size_t size) {
  const size_t middle = size / 2;
  size_t out = 0;
  size_t i = 0;
  size_t j = middle;

  /* don't bother sorting an array of size <= 1 */
  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(dst, size);
    return;
  }

  MERGE_SORT_RECURSIVE(newdst, dst, middle);
  MERGE_SORT_RECURSIVE(newdst, &dst[middle], size - middle);

  while (out != size) {
    if (i < middle) {
      if (j < size) {
        if (SORT_CMP(dst[i], dst[j]) <= 0) {
          newdst[out] = dst[i++];
        } else {
          newdst[out] = dst[j++];
        }
      } else {
        newdst[out] = dst[i++];
      }
    } else {
      newdst[out] = dst[j++];
    }

    out++;
  }

  SORT_TYPE_CPY(dst, newdst, size);
}

/* Standard merge sort */
SORT_DEF void MERGE_SORT(SORT_TYPE *dst, const size_t size) {
  SORT_TYPE *newdst;

  /* don't bother sorting an array of size <= 1 */
  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(dst, size);
    return;
  }

  newdst = SORT_NEW_BUFFER(size);
  MERGE_SORT_RECURSIVE(newdst, dst, size);
  SORT_DELETE_BUFFER(newdst);
}

static __inline size_t QUICK_SORT_PARTITION(SORT_TYPE *dst, const size_t left,
    const size_t right, const size_t pivot) {
  SORT_TYPE value = dst[pivot];
  size_t index = left;
  size_t i;
  int not_all_same = 0;
  /* move the pivot to the right */
  SORT_SWAP(dst[pivot], dst[right]);

  for (i = left; i < right; i++) {
    int cmp = SORT_CMP(dst[i], value);
    /* check if everything is all the same */
    not_all_same |= cmp;

    if (cmp < 0) {
      SORT_SWAP(dst[i], dst[index]);
      index++;
    }
  }

  SORT_SWAP(dst[right], dst[index]);

  /* avoid degenerate case */
  if (not_all_same == 0) {
    return SIZE_MAX;
  }

  return index;
}

/* Return the median index of the objects at the three indices. */
static __inline size_t MEDIAN(const SORT_TYPE *dst, const size_t a, const size_t b,
                              const size_t c) {
  const int AB = SORT_CMP(dst[a], dst[b]) < 0;

  if (AB) {
    /* a < b */
    const int BC = SORT_CMP(dst[b], dst[c]) < 0;

    if (BC) {
      /* a < b < c */
      return b;
    } else {
      /* a < b, c < b */
      const int AC = SORT_CMP(dst[a], dst[c]) < 0;

      if (AC) {
        /* a < c < b */
        return c;
      } else {
        /* c < a < b */
        return a;
      }
    }
  } else {
    /* b < a */
    const int AC = SORT_CMP(dst[a], dst[b]) < 0;

    if (AC) {
      /* b < a < c */
      return a;
    } else {
      /* b < a, c < a */
      const int BC = SORT_CMP(dst[b], dst[c]) < 0;

      if (BC) {
        /* b < c < a */
        return c;
      } else {
        /* c < b < a */
        return b;
      }
    }
  }
}

static void QUICK_SORT_RECURSIVE(SORT_TYPE *dst, const size_t original_left,
                                 const size_t original_right) {
  size_t left;
  size_t right;
  size_t pivot;
  size_t new_pivot;
  size_t middle;
  int loop_count = 0;
  const int max_loops = 64 - CLZ(original_right - original_left); /* ~lg N */
  left = original_left;
  right = original_right;

  while (1) {
    if (right <= left) {
      return;
    }

    if ((right - left + 1U) <= SMALL_SORT_BND) {
      SMALL_SORT(&dst[left], right - left + 1U);
      return;
    }

    if (++loop_count >= max_loops) {
      /* we have recursed / looped too many times; switch to heap sort */
      HEAP_SORT(&dst[left], right - left + 1U);
      return;
    }

    /* median of 5 */
    middle = left + ((right - left) >> 1);
    pivot = MEDIAN((const SORT_TYPE *) dst, left, middle, right);
    pivot = MEDIAN((const SORT_TYPE *) dst, left + ((middle - left) >> 1), pivot,
                   middle + ((right - middle) >> 1));
    new_pivot = QUICK_SORT_PARTITION(dst, left, right, pivot);

    /* check for partition all equal */
    if (new_pivot == SIZE_MAX) {
      return;
    }

    /* recurse only on the small part to avoid degenerate stack sizes */
    /* and manually do tail call on the large part */
    if (new_pivot - 1U - left > right - new_pivot - 1U) {
      /* left is bigger than right */
      QUICK_SORT_RECURSIVE(dst, new_pivot + 1U, right);
      /* tail call for left */
      right = new_pivot - 1U;
    } else {
      /* right is bigger than left */
      QUICK_SORT_RECURSIVE(dst, left, new_pivot - 1U);
      /* tail call for right */
      left = new_pivot + 1U;
    }
  }
}

void QUICK_SORT(SORT_TYPE *dst, const size_t size) {
  /* don't bother sorting an array of size 1 */
  if (size <= 1) {
    return;
  }

  QUICK_SORT_RECURSIVE(dst, 0U, size - 1U);
}


/* timsort implementation, based on timsort.txt */

static __inline void REVERSE_ELEMENTS(SORT_TYPE *dst, size_t start, size_t end) {
  while (1) {
    if (start >= end) {
      return;
    }

    SORT_SWAP(dst[start], dst[end]);
    start++;
    end--;
  }
}

/* heap sort: based on wikipedia */

static __inline void HEAP_SIFT_DOWN(SORT_TYPE *dst, const size_t start, const size_t end) {
  size_t root = start;

  while ((root << 1) <= end) {
    size_t child = root << 1;

    if ((child < end) && (SORT_CMP(dst[child], dst[child + 1]) < 0)) {
      child++;
    }

    if (SORT_CMP(dst[root], dst[child]) < 0) {
      SORT_SWAP(dst[root], dst[child]);
      root = child;
    } else {
      return;
    }
  }
}

static __inline void HEAPIFY(SORT_TYPE *dst, const size_t size) {
  size_t start = size >> 1;

  while (1) {
    HEAP_SIFT_DOWN(dst, start, size - 1);

    if (start == 0) {
      break;
    }

    start--;
  }
}

SORT_DEF void HEAP_SORT(SORT_TYPE *dst, const size_t size) {
  size_t end = size - 1;

  /* don't bother sorting an array of size <= 1 */
  if (size <= 1) {
    return;
  }

  HEAPIFY(dst, size);

  while (end > 0) {
    SORT_SWAP(dst[end], dst[0]);
    HEAP_SIFT_DOWN(dst, 0, end - 1);
    end--;
  }
}

#include "merge_sorts.h"

#undef SORT_SAFE_CPY
#undef SORT_TYPE_CPY
#undef SORT_TYPE_MOVE
#undef SORT_NEW_BUFFER
#undef SORT_DELETE_BUFFER
#undef QUICK_SORT
#undef MEDIAN
#undef SORT_CONCAT
#undef SORT_MAKE_STR1
#undef SORT_MAKE_STR
#undef SORT_NAME
#undef SORT_TYPE
#undef SORT_CMP
#undef SORT_SWAP
#undef SORT_MAKE_STR1
#undef SORT_MAKE_STR
#undef BINARY_INSERTION_FIND
#undef BINARY_INSERTION_SORT_START
#undef BINARY_INSERTION_SORT
#undef REVERSE_ELEMENTS
#undef MERGE_SORT
#undef MERGE_SORT_RECURSIVE

#ifdef SORT_GCC_PUSH
#undef SORT_GCC_PUSH
#pragma GCC diagnostic pop
#endif
