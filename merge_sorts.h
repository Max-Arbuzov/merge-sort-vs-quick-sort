/* Copyright (c) 2010-2024 Christopher Swenson. */
/* Copyright (c) 2012 Vojtech Fried. */
/* Copyright (c) 2012 Google Inc. All Rights Reserved. */

#define MERGE_SORT_STD            SORT_MAKE_STR(merge_sort_std)
#define MERGE_SORT_STD_RECURSIVE  SORT_MAKE_STR(merge_sort_std_recursive)
// studying ways to work with temp buffer
#define MERGE_SORT_HALVED                   SORT_MAKE_STR(merge_sort_halved)
#define MERGE_SORT_HALVED_RECURSIVE         SORT_MAKE_STR(merge_sort_halved_recursive)
#define MERGE_SORT_NOCOPY                   SORT_MAKE_STR(merge_sort_nocopy)
#define MERGE_SORT_NOCOPY_RECURSIVE         SORT_MAKE_STR(merge_sort_nocopy_recursive)
#define MERGE_SORT_HALVED_NOCOPY            SORT_MAKE_STR(merge_sort_halved_nocopy)
#define MERGE_SORT_HALVED_NOCOPY_RECURSIVE  SORT_MAKE_STR(merge_sort_halved_nocopy_recursive)
#define MERGE_SORT_UNIFORMBUFFER            SORT_MAKE_STR(merge_sort_uniformbuffer)
#define MERGE_SORT_UNIFORMBUFFER_RECURSIVE  SORT_MAKE_STR(merge_sort_uniformbuffer_recursive)
// studying ways to handle small sub-arrays
#define MERGE_SORT_NORECURSION                 SORT_MAKE_STR(merge_sort_norecursion)
#define MERGE_SORT_NORECURSION_INNER           SORT_MAKE_STR(merge_sort_norecursion_inner)
#define MERGE_SORT_NORECURSION_MERGECHUNKS     SORT_MAKE_STR(merge_sort_norecursion_mergechunks)
#define MERGE_SORT_NORECURSION_PRESORT2        SORT_MAKE_STR(merge_sort_norecursion_presort2)
#define MERGE_SORT_NORECURSION_PRESORT2_INNER  SORT_MAKE_STR(merge_sort_norecursion_presort2_inner)

SORT_DEF void MERGE_SORT_STD(SORT_TYPE *dst, const size_t size);
SORT_DEF void MERGE_SORT_HALVED(SORT_TYPE *dst, const size_t size);
SORT_DEF void MERGE_SORT_NOCOPY(SORT_TYPE *dst, const size_t size);
SORT_DEF void MERGE_SORT_HALVED_NOCOPY(SORT_TYPE *dst, const size_t size);
SORT_DEF void MERGE_SORT_UNIFORMBUFFER(SORT_TYPE *dst, const size_t size);
SORT_DEF void MERGE_SORT_NORECURSION(SORT_TYPE *dst, const size_t size);
SORT_DEF void MERGE_SORT_NORECURSION_PRESORT2(SORT_TYPE *dst, const size_t size);

#define STABLE_SORT_2  SORT_MAKE_STR(stable_sort_2)
static __inline void STABLE_SORT_2(SORT_TYPE *data) {
  SORT_TYPE item_0 = data[0];
  SORT_TYPE item_1 = data[1];
  if (SORT_CMP(item_0, item_1) > 0) {
    data[0] = item_1;
    data[1] = item_0;
  }
}

/***********************\
 * Standard merge sort *
\***********************/

SORT_DEF void MERGE_SORT_STD_RECURSIVE(SORT_TYPE *data, SORT_TYPE *temp, const size_t size) {
  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(data, size);
    return;
  }

  const size_t middle = size >> 1;
  MERGE_SORT_STD_RECURSIVE(data, temp, middle);
  MERGE_SORT_STD_RECURSIVE(&data[middle], temp, size - middle);

  size_t out = 0;
  size_t i = 0;
  size_t j = middle;
  while ((i < middle) && (j < size)) {
    if (SORT_CMP(data[i], data[j]) <= 0) {
      temp[out] = data[i++];
    } else {
      temp[out] = data[j++];
    }
    ++out;
  }
  while (i < middle) {
    temp[out++] = data[i++];
  }
  while (j < size) {
    temp[out++] = data[j++];
  }

  SORT_TYPE_CPY(data, temp, size);
}

SORT_DEF void MERGE_SORT_STD(SORT_TYPE *dst, const size_t size) {
  SORT_TYPE *tempBuf;

  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(dst, size);
    return;
  }

  tempBuf = SORT_NEW_BUFFER(size);
  MERGE_SORT_STD_RECURSIVE(dst, tempBuf, size);
  SORT_DELETE_BUFFER(tempBuf);
}

/**************************************\
 * Merge sort with halved temp buffer *
\**************************************/

SORT_DEF void MERGE_SORT_HALVED_RECURSIVE(SORT_TYPE *data, SORT_TYPE *temp, const size_t size) {
  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(data, size);
    return;
  }

  const size_t middle = size >> 1;
  MERGE_SORT_HALVED_RECURSIVE(data, temp, middle);
  MERGE_SORT_HALVED_RECURSIVE(&data[middle], temp, size - middle);

  SORT_TYPE_CPY(temp, data, middle);

  size_t out = 0;
  size_t i = 0;
  size_t j = middle;
  while ((i < middle) && (j < size)) {
    if (SORT_CMP(temp[i], data[j]) <= 0) {
      data[out] = temp[i++];
    } else {
      data[out] = data[j++];
    }
    ++out;
  }
  while (i < middle) {
    data[out++] = temp[i++];
  }
  while (j < size) {
    data[out++] = data[j++];
  }
}

SORT_DEF void MERGE_SORT_HALVED(SORT_TYPE *dst, const size_t size) {
  SORT_TYPE *tempBuf;

  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(dst, size);
    return;
  }

  tempBuf = SORT_NEW_BUFFER(size/2);
  MERGE_SORT_HALVED_RECURSIVE(dst, tempBuf, size);
  SORT_DELETE_BUFFER(tempBuf);
}

/***********************************************\
 * Merge sort without copying from temp buffer *
\***********************************************/

//takes values from data or temp according to sourceIndex, sort values and put sorted values into data
//if sourceIndex == 0 then takes from data
//if sourceIndex == 1 then takes from temp
SORT_DEF void MERGE_SORT_NOCOPY_RECURSIVE(SORT_TYPE *data, SORT_TYPE *temp, const size_t size, int sourceIndex) {
  if (size <= 1) {
    if ((sourceIndex > 0) && (size > 0))
      data[0] = temp[0];
    return;
  }

  if (size <= SMALL_SORT_BND) {
    if (sourceIndex > 0)
      SORT_TYPE_CPY(data, temp, size);
    SMALL_STABLE_SORT(data, size);
    return;
  }

  const size_t middle = size >> 1;
  MERGE_SORT_NOCOPY_RECURSIVE(temp, data, middle, 1 - sourceIndex);
  MERGE_SORT_NOCOPY_RECURSIVE(&temp[middle], &data[middle], size - middle, 1 - sourceIndex);

  size_t out = 0;
  size_t i = 0;
  size_t j = middle;
  while ((i < middle) && (j < size)) {
    if (SORT_CMP(temp[i], temp[j]) <= 0) {
      data[out] = temp[i++];
    } else {
      data[out] = temp[j++];
    }
    ++out;
  }
  while (i < middle) {
    data[out++] = temp[i++];
  }
  while (j < size) {
    data[out++] = temp[j++];
  }
}

SORT_DEF void MERGE_SORT_NOCOPY(SORT_TYPE *dst, const size_t size) {
  SORT_TYPE *tempBuf;

  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(dst, size);
    return;
  }

  tempBuf = SORT_NEW_BUFFER(size);
  MERGE_SORT_NOCOPY_RECURSIVE(dst, tempBuf, size, 0);
  SORT_DELETE_BUFFER(tempBuf);
}

/*************************************************************************\
 * Merge sort with halved temp buffer and without copying to temp buffer *
\*************************************************************************/

//takes values from data or temp according to sourceIndex, sort values and put sorted values into data
//if sourceIndex == 0 then takes from data
//if sourceIndex == 1 then takes from temp
SORT_DEF void MERGE_SORT_HALVED_NOCOPY_RECURSIVE(SORT_TYPE *data, SORT_TYPE *temp, const size_t size, int sourceIndex) {
  if (size <= 1) {
    if ((sourceIndex > 0) && (size > 0))
      data[0] = temp[0];
    return;
  }

  if (size <= SMALL_SORT_BND) {
    if (sourceIndex > 0)
      SORT_TYPE_CPY(data, temp, size);
    SMALL_STABLE_SORT(data, size);
    return;
  }

  const size_t middle = size >> 1;
  if (sourceIndex <= 0) {
    MERGE_SORT_HALVED_NOCOPY_RECURSIVE(&data[middle], temp, size - middle, sourceIndex);
  } else {
    MERGE_SORT_HALVED_NOCOPY_RECURSIVE(&data[middle], &temp[middle], size - middle, sourceIndex);
  }
  MERGE_SORT_HALVED_NOCOPY_RECURSIVE(temp, data, middle, 1 - sourceIndex);

  size_t out = 0;
  size_t i = 0;
  size_t j = middle;
  while ((i < middle) && (j < size)) {
    if (SORT_CMP(temp[i], data[j]) <= 0) {
      data[out] = temp[i++];
    } else {
      data[out] = data[j++];
    }
    ++out;
  }
  while (i < middle) {
    data[out++] = temp[i++];
  }
  while (j < size) {
    data[out++] = data[j++];
  }
}

SORT_DEF void MERGE_SORT_HALVED_NOCOPY(SORT_TYPE *dst, const size_t size) {
  SORT_TYPE *tempBuf;

  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(dst, size);
    return;
  }

  tempBuf = SORT_NEW_BUFFER(size/2);
  MERGE_SORT_HALVED_NOCOPY_RECURSIVE(dst, tempBuf, size, 0);
  SORT_DELETE_BUFFER(tempBuf);
}

/*******************************************************\
 * Standard merge sort that uses temp buffer uniformly *
\*******************************************************/

SORT_DEF void MERGE_SORT_UNIFORMBUFFER_RECURSIVE(SORT_TYPE *data, SORT_TYPE *temp, const size_t size) {
  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(data, size);
    return;
  }

  const size_t middle = size >> 1;
  MERGE_SORT_UNIFORMBUFFER_RECURSIVE(data, temp, middle);
  MERGE_SORT_UNIFORMBUFFER_RECURSIVE(&data[middle], &temp[middle], size - middle);

  size_t out = 0;
  size_t i = 0;
  size_t j = middle;
  while ((i < middle) && (j < size)) {
    if (SORT_CMP(data[i], data[j]) <= 0) {
      temp[out] = data[i++];
    } else {
      temp[out] = data[j++];
    }
    ++out;
  }
  while (i < middle) {
    temp[out++] = data[i++];
  }
  while (j < size) {
    temp[out++] = data[j++];
  }

  SORT_TYPE_CPY(data, temp, size);
}

SORT_DEF void MERGE_SORT_UNIFORMBUFFER(SORT_TYPE *dst, const size_t size) {
  SORT_TYPE *tempBuf;

  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(dst, size);
    return;
  }

  tempBuf = SORT_NEW_BUFFER(size);
  MERGE_SORT_UNIFORMBUFFER_RECURSIVE(dst, tempBuf, size);
  SORT_DELETE_BUFFER(tempBuf);
}

/********************************************\
 * Merge sort without recursion (bottom-up) *
\********************************************/

//merges two adjacent chunks into one for all chunks in the array
static __inline void MERGE_SORT_NORECURSION_MERGECHUNKS(SORT_TYPE *dest, SORT_TYPE *source, const size_t size, const size_t chunkSize) {
  size_t out = 0;
  while (out < size) {
    size_t i = out;
    size_t iEnd = out + chunkSize;
    size_t j = iEnd;
    size_t jEnd = iEnd + chunkSize;
    if (iEnd > size)
      iEnd = size;
    if (jEnd > size)
      jEnd = size;

    while ((i < iEnd) && (j < jEnd)) {
      if (SORT_CMP(source[i], source[j]) <= 0) {
        dest[out] = source[i++];
      } else {
        dest[out] = source[j++];
      }
      ++out;
    }
    while (i < iEnd) {
      dest[out++] = source[i++];
    }
    while (j < jEnd) {
      dest[out++] = source[j++];
    }
  }
}

//takes values from data or temp according to sourceIndex, sort values and put sorted values into data
//if sourceIndex == 0 then takes from data
//if sourceIndex == 1 then takes from temp
SORT_DEF void MERGE_SORT_NORECURSION_INNER(SORT_TYPE *data, SORT_TYPE *temp, const size_t size, int sourceIndex) {
  size_t chunkSize = 1;
  while (chunkSize < size) {
    if (sourceIndex <= 0) {
      MERGE_SORT_NORECURSION_MERGECHUNKS(temp, data, size, chunkSize);
    } else {
      MERGE_SORT_NORECURSION_MERGECHUNKS(data, temp, size, chunkSize);
    }
    sourceIndex = 1 - sourceIndex;
    chunkSize *= 2;
  }

  if (sourceIndex > 0)
    SORT_TYPE_CPY(data, temp, size);
}

SORT_DEF void MERGE_SORT_NORECURSION(SORT_TYPE *dst, const size_t size) {
  SORT_TYPE *tempBuf;

  if (size <= 1) {
    return;
  }

  tempBuf = SORT_NEW_BUFFER(size);
  MERGE_SORT_NORECURSION_INNER(dst, tempBuf, size, 0);
  SORT_DELETE_BUFFER(tempBuf);
}

/*********************************************************************\
 * Merge sort without recursion (bottom-up) with presorting of pairs *
\*********************************************************************/

//takes values from data or temp according to sourceIndex, sort values and put sorted values into data
//if sourceIndex == 0 then takes from data
//if sourceIndex == 1 then takes from temp
SORT_DEF void MERGE_SORT_NORECURSION_PRESORT2_INNER(SORT_TYPE *data, SORT_TYPE *temp, const size_t size, int sourceIndex) {
  size_t i;
  for (i = 2; i <= size; i += 2) {
    STABLE_SORT_2(&data[i - 2U]);
  }
  size_t chunkSize = 2;

  while (chunkSize < size) {
    if (sourceIndex <= 0) {
      MERGE_SORT_NORECURSION_MERGECHUNKS(temp, data, size, chunkSize);
    } else {
      MERGE_SORT_NORECURSION_MERGECHUNKS(data, temp, size, chunkSize);
    }
    sourceIndex = 1 - sourceIndex;
    chunkSize *= 2;
  }

  if (sourceIndex > 0)
    SORT_TYPE_CPY(data, temp, size);
}

SORT_DEF void MERGE_SORT_NORECURSION_PRESORT2(SORT_TYPE *dst, const size_t size) {
  SORT_TYPE *tempBuf;

  if (size <= 1) {
    return;
  }

  tempBuf = SORT_NEW_BUFFER(size);
  MERGE_SORT_NORECURSION_PRESORT2_INNER(dst, tempBuf, size, 0);
  SORT_DELETE_BUFFER(tempBuf);
}

#undef STABLE_SORT_2

#undef MERGE_SORT_STD
#undef MERGE_SORT_STD_RECURSIVE

#undef MERGE_SORT_HALVED
#undef MERGE_SORT_HALVED_RECURSIVE
#undef MERGE_SORT_NOCOPY
#undef MERGE_SORT_NOCOPY_RECURSIVE
#undef MERGE_SORT_HALVED_NOCOPY
#undef MERGE_SORT_HALVED_NOCOPY_RECURSIVE
#undef MERGE_SORT_UNIFORMBUFFER
#undef MERGE_SORT_UNIFORMBUFFER_RECURSIVE

#undef MERGE_SORT_NORECURSION
#undef MERGE_SORT_NORECURSION_INNER
#undef MERGE_SORT_NORECURSION_MERGECHUNKS
#undef MERGE_SORT_NORECURSION_PRESORT2
#undef MERGE_SORT_NORECURSION_PRESORT2_INNER
