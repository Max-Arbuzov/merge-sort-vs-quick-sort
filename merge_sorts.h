/* Copyright (c) 2010-2024 Christopher Swenson. */
/* Copyright (c) 2012 Vojtech Fried. */
/* Copyright (c) 2012 Google Inc. All Rights Reserved. */

#define MERGE_SORT_STD            SORT_MAKE_STR(merge_sort_std)
#define MERGE_SORT_STD_RECURSIVE  SORT_MAKE_STR(merge_sort_std_recursive)

#define MERGE_SORT_HALVED            SORT_MAKE_STR(merge_sort_halved)
#define MERGE_SORT_HALVED_RECURSIVE  SORT_MAKE_STR(merge_sort_halved_recursive)

SORT_DEF void MERGE_SORT_STD(SORT_TYPE *dst, const size_t size);
SORT_DEF void MERGE_SORT_HALVED(SORT_TYPE *dst, const size_t size);

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

#undef MERGE_SORT_STD
#undef MERGE_SORT_STD_RECURSIVE

#undef MERGE_SORT_HALVED
#undef MERGE_SORT_HALVED_RECURSIVE
