#define STABLE_PRESORT             SORT_MAKE_STR(stable_presort)
#define MERGE_SORT_SMALL           SORT_MAKE_STR(merge_sort_small)
#define MERGE_SORT_SMALL_BALANCED  SORT_MAKE_STR(merge_sort_small_balanced)

#define MERGE_CHUNK_INPLACE  SORT_MAKE_STR(merge_chunk_inplace)
// merges two adjacent chunks into one
// i - left start
// j - right start
// end - start of the next chunk that should not be processed here
// O(N*N) item moves
static __inline void MERGE_CHUNK_INPLACE(SORT_TYPE *data, size_t i, size_t j, size_t end) {
  while ((i < j) && (j < end)) {
    if (SORT_CMP(data[i], data[j]) > 0) {
      SORT_TYPE item = data[j];
      for (int k = j; k > i; k--) {
        data[k] = data[k - 1];
      }
      data[i] = item;
      ++j;
    }
    ++i;
  }
}

// sorted size is 0 before call and 1 after
// data[0] has no data before call
#define INSERT_INTO_SORTED_1  SORT_MAKE_STR(insert_into_sorted_1)
static __inline void INSERT_INTO_SORTED_1(SORT_TYPE *data, SORT_TYPE newItem) {
  data[0] = newItem;
}

// sorted size is 1 before call and 2 after
// data[1] has no data before call
#define INSERT_INTO_SORTED_2  SORT_MAKE_STR(insert_into_sorted_2)
static __inline void INSERT_INTO_SORTED_2(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_0 = data[0];
  if (SORT_CMP(item_0, newItem) > 0) {
    data[1] = item_0;
    data[0] = newItem; // INSERT_INTO_SORTED_1(data, newItem);
  } else {
    data[1] = newItem; // INSERT_INTO_SORTED_1(&data[1], newItem);
  }
}

// sorted size is 2 before call and 3 after
// data[2] has no data before call
#define INSERT_INTO_SORTED_3  SORT_MAKE_STR(insert_into_sorted_3)
static __inline void INSERT_INTO_SORTED_3(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_1 = data[1];
  if (SORT_CMP(item_1, newItem) > 0) {
    data[2] = item_1;
    INSERT_INTO_SORTED_2(data, newItem);
  } else {
    data[2] = newItem; // INSERT_INTO_SORTED_1(&data[2], newItem);
  }
}

// sorted size is 3 before call and 4 after
// data[3] has no data before call
#define INSERT_INTO_SORTED_4  SORT_MAKE_STR(insert_into_sorted_4)
static __inline void INSERT_INTO_SORTED_4(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_1 = data[1];
  if (SORT_CMP(item_1, newItem) > 0) {
    data[3] = data[2];
    data[2] = item_1;
    INSERT_INTO_SORTED_2(data, newItem);
  } else {
    INSERT_INTO_SORTED_2(&data[2], newItem);
  }
}

#define STABLE_SORT_1  SORT_MAKE_STR(stable_sort_1)
static __inline void STABLE_SORT_1(SORT_TYPE *data) {
  return;
}

#define STABLE_SORT_2  SORT_MAKE_STR(stable_sort_2)
static __inline void STABLE_SORT_2(SORT_TYPE *data) {
  // STABLE_SORT_1(data);
  SORT_TYPE item_0 = data[0];
  SORT_TYPE item_1 = data[1];
  if (SORT_CMP(item_0, item_1) > 0) {
    data[1] = item_0;
    data[0] = item_1; // INSERT_INTO_SORTED_1(data, item_1);
  } else {
    // INSERT_INTO_SORTED_1(&data[1], item_1);
  }
}

#define STABLE_SORT_3  SORT_MAKE_STR(stable_sort_3)
static __inline void STABLE_SORT_3(SORT_TYPE *data) {
  STABLE_SORT_2(data);
  SORT_TYPE item_1 = data[1];
  SORT_TYPE item_2 = data[2];
  if (SORT_CMP(item_1, item_2) > 0) {
    data[2] = item_1;
    INSERT_INTO_SORTED_2(data, item_2);
  } else {
    //INSERT_INTO_SORTED_1(&data[2], item_2);
  }
}

#define STABLE_SORT_4  SORT_MAKE_STR(stable_sort_4)
static __inline void STABLE_SORT_4(SORT_TYPE *data) {
  STABLE_SORT_3(data);
  SORT_TYPE item_1 = data[1];
  SORT_TYPE item_3 = data[3];
  if (SORT_CMP(item_1, item_3) > 0) {
    data[3] = data[2];
    data[2] = item_1;
    INSERT_INTO_SORTED_2(data, item_3);
  } else {
    INSERT_INTO_SORTED_2(&data[2], item_3);
  }
}

//sorts items in chunks of the array for subsequent bottom-up merging
//chunkSize can be 2, 3 or 4
//returns actual size of sorted chunks
static __inline int STABLE_PRESORT(SORT_TYPE *data, const size_t size, size_t chunkSize) {
  size_t i;

  if (chunkSize == 2) {
    for (i = 2; i <= size; i += 2) {
      STABLE_SORT_2(&data[i - 2U]);
    }
    i -= 2;
  } else if (chunkSize == 3) {
    for (i = 3; i <= size; i += 3) {
      STABLE_SORT_3(&data[i - 3U]);
    }
    i -= 3;
  } else if (chunkSize == 4) {
    for (i = 4; i <= size; i += 4) {
      STABLE_SORT_4(&data[i - 4U]);
    }
    i -= 4;
  } else {
    return 1;
  }

  size_t remain = size - i;
  if (remain == 2) {
    STABLE_SORT_2(&data[i]);
  } else if (remain == 3) {
    STABLE_SORT_3(&data[i]);
  }
  return chunkSize;
}

/******************************************\
 * Merge sort for small arrays            *
 * in-place without recursion (bottom-up) *
 * O(N*logN) comparisons                  *
 * O(N*N) item moves                      *
\******************************************/

#define MERGE_SORT_SMALL_MERGECHUNKS  SORT_MAKE_STR(merge_sort_small_mergechunks)
//merges two adjacent chunks into one for all chunks in the array
static __inline void MERGE_SORT_SMALL_MERGECHUNKS(SORT_TYPE *data, const size_t size, const size_t chunkSize) {
  size_t end = 0;
  while (end < size) {
    size_t i = end;
    size_t j = end + chunkSize;
    end = j + chunkSize;
    if (end > size)
      end = size;

    MERGE_CHUNK_INPLACE(data, i, j, end);
  }
}

SORT_DEF void MERGE_SORT_SMALL(SORT_TYPE *data, const size_t size) {
  size_t chunkSize = STABLE_PRESORT(data, size, 4);
  while (chunkSize < size) {
    MERGE_SORT_SMALL_MERGECHUNKS(data, size, chunkSize);
    chunkSize *= 2;
  }
}

//balanced version of MERGE_SORT_SMALL()
SORT_DEF void MERGE_SORT_SMALL_BALANCED(SORT_TYPE *data, const size_t size) {
  size_t chunkSize = 4;
  if ((size > 8) && (size < 13))
    chunkSize = 3;
  chunkSize = STABLE_PRESORT(data, size, chunkSize);
  while (chunkSize < size) {
    MERGE_SORT_SMALL_MERGECHUNKS(data, size, chunkSize);
    chunkSize *= 2;
  }
}

#undef MERGE_CHUNK_INPLACE
#undef INSERT_INTO_SORTED_1
#undef INSERT_INTO_SORTED_2
#undef INSERT_INTO_SORTED_3
#undef INSERT_INTO_SORTED_4

#undef STABLE_SORT_1
#undef STABLE_SORT_2
#undef STABLE_SORT_3
#undef STABLE_SORT_4
#undef STABLE_PRESORT

#undef MERGE_SORT_SMALL_MERGECHUNKS
#undef MERGE_SORT_SMALL
#undef MERGE_SORT_SMALL_BALANCED
