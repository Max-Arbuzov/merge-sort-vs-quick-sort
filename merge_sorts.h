/* Copyright (c) 2010-2024 Christopher Swenson. */
/* Copyright (c) 2012 Vojtech Fried. */
/* Copyright (c) 2012 Google Inc. All Rights Reserved. */

#define MERGE_SORT_STD            SORT_MAKE_STR(merge_sort_std)
#define MERGE_SORT_STD_RECURSIVE  SORT_MAKE_STR(merge_sort_std_recursive)

SORT_DEF void MERGE_SORT_STD(SORT_TYPE *dst, const size_t size);

/***********************\
 * Standard merge sort *
\***********************/

SORT_DEF void MERGE_SORT_STD_RECURSIVE(SORT_TYPE *data, SORT_TYPE *temp, const size_t size) {
  const size_t middle = size / 2;
  size_t out = 0;
  size_t i = 0;
  size_t j = middle;

  if (size <= 1) {
    return;
  }

  if (size <= SMALL_SORT_BND) {
    SMALL_STABLE_SORT(data, size);
    return;
  }

  MERGE_SORT_STD_RECURSIVE(data, temp, middle);
  MERGE_SORT_STD_RECURSIVE(&data[middle], temp, size - middle);

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

#undef MERGE_SORT_STD
#undef MERGE_SORT_STD_RECURSIVE
