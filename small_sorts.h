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

// sorted size is 4 before call and 5 after
// data[4] has no data before call
#define INSERT_INTO_SORTED_5  SORT_MAKE_STR(insert_into_sorted_5)
static __inline void INSERT_INTO_SORTED_5(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_2 = data[2];
  if (SORT_CMP(item_2, newItem) > 0) {
    data[4] = data[3];
    data[3] = item_2;
    INSERT_INTO_SORTED_3(data, newItem);
  } else {
    INSERT_INTO_SORTED_2(&data[3], newItem);
  }
}

// sorted size is 5 before call and 6 after
// data[5] has no data before call
#define INSERT_INTO_SORTED_6  SORT_MAKE_STR(insert_into_sorted_6)
static __inline void INSERT_INTO_SORTED_6(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_2 = data[2];
  if (SORT_CMP(item_2, newItem) > 0) {
    data[5] = data[4];
    data[4] = data[3];
    data[3] = item_2;
    INSERT_INTO_SORTED_3(data, newItem);
  } else {
    INSERT_INTO_SORTED_3(&data[3], newItem);
  }
}

// sorted size is 6 before call and 7 after
// data[6] has no data before call
#define INSERT_INTO_SORTED_7  SORT_MAKE_STR(insert_into_sorted_7)
static __inline void INSERT_INTO_SORTED_7(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_3 = data[3];
  if (SORT_CMP(item_3, newItem) > 0) {
    data[6] = data[5];
    data[5] = data[4];
    data[4] = item_3;
    INSERT_INTO_SORTED_4(data, newItem);
  } else {
    INSERT_INTO_SORTED_3(&data[4], newItem);
  }
}

// sorted size is 7 before call and 8 after
// data[7] has no data before call
#define INSERT_INTO_SORTED_8  SORT_MAKE_STR(insert_into_sorted_8)
static __inline void INSERT_INTO_SORTED_8(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_3 = data[3];
  if (SORT_CMP(item_3, newItem) > 0) {
    data[7] = data[6];
    data[6] = data[5];
    data[5] = data[4];
    data[4] = item_3;
    INSERT_INTO_SORTED_4(data, newItem);
  } else {
    INSERT_INTO_SORTED_4(&data[4], newItem);
  }
}

// sorted size is 8 before call and 9 after
// data[8] has no data before call
#define INSERT_INTO_SORTED_9  SORT_MAKE_STR(insert_into_sorted_9)
static __inline void INSERT_INTO_SORTED_9(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_4 = data[4];
  if (SORT_CMP(item_4, newItem) > 0) {
    data[8] = data[7];
    data[7] = data[6];
    data[6] = data[5];
    data[5] = item_4;
    INSERT_INTO_SORTED_5(data, newItem);
  } else {
    INSERT_INTO_SORTED_4(&data[5], newItem);
  }
}

// sorted size is 9 before call and 10 after
// data[9] has no data before call
#define INSERT_INTO_SORTED_10  SORT_MAKE_STR(insert_into_sorted_10)
static __inline void INSERT_INTO_SORTED_10(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_4 = data[4];
  if (SORT_CMP(item_4, newItem) > 0) {
    data[9] = data[8];
    data[8] = data[7];
    data[7] = data[6];
    data[6] = data[5];
    data[5] = item_4;
    INSERT_INTO_SORTED_5(data, newItem);
  } else {
    INSERT_INTO_SORTED_5(&data[5], newItem);
  }
}

// sorted size is 10 before call and 11 after
// data[10] has no data before call
#define INSERT_INTO_SORTED_11  SORT_MAKE_STR(insert_into_sorted_11)
static __inline void INSERT_INTO_SORTED_11(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_5 = data[5];
  if (SORT_CMP(item_5, newItem) > 0) {
    data[10] = data[9];
    data[9] = data[8];
    data[8] = data[7];
    data[7] = data[6];
    data[6] = item_5;
    INSERT_INTO_SORTED_6(data, newItem);
  } else {
    INSERT_INTO_SORTED_5(&data[6], newItem);
  }
}

// sorted size is 11 before call and 12 after
// data[11] has no data before call
#define INSERT_INTO_SORTED_12  SORT_MAKE_STR(insert_into_sorted_12)
static __inline void INSERT_INTO_SORTED_12(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_5 = data[5];
  if (SORT_CMP(item_5, newItem) > 0) {
    data[11] = data[10];
    data[10] = data[9];
    data[9] = data[8];
    data[8] = data[7];
    data[7] = data[6];
    data[6] = item_5;
    INSERT_INTO_SORTED_6(data, newItem);
  } else {
    INSERT_INTO_SORTED_6(&data[6], newItem);
  }
}

// sorted size is 12 before call and 13 after
// data[12] has no data before call
#define INSERT_INTO_SORTED_13  SORT_MAKE_STR(insert_into_sorted_13)
static __inline void INSERT_INTO_SORTED_13(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_6 = data[6];
  if (SORT_CMP(item_6, newItem) > 0) {
    data[12] = data[11];
    data[11] = data[10];
    data[10] = data[9];
    data[9] = data[8];
    data[8] = data[7];
    data[7] = item_6;
    INSERT_INTO_SORTED_7(data, newItem);
  } else {
    INSERT_INTO_SORTED_6(&data[7], newItem);
  }
}

// sorted size is 13 before call and 14 after
// data[13] has no data before call
#define INSERT_INTO_SORTED_14  SORT_MAKE_STR(insert_into_sorted_14)
static __inline void INSERT_INTO_SORTED_14(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_6 = data[6];
  if (SORT_CMP(item_6, newItem) > 0) {
    data[13] = data[12];
    data[12] = data[11];
    data[11] = data[10];
    data[10] = data[9];
    data[9] = data[8];
    data[8] = data[7];
    data[7] = item_6;
    INSERT_INTO_SORTED_7(data, newItem);
  } else {
    INSERT_INTO_SORTED_7(&data[7], newItem);
  }
}

// sorted size is 14 before call and 15 after
// data[14] has no data before call
#define INSERT_INTO_SORTED_15  SORT_MAKE_STR(insert_into_sorted_15)
static __inline void INSERT_INTO_SORTED_15(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_7 = data[7];
  if (SORT_CMP(item_7, newItem) > 0) {
    data[14] = data[13];
    data[13] = data[12];
    data[12] = data[11];
    data[11] = data[10];
    data[10] = data[9];
    data[9] = data[8];
    data[8] = item_7;
    INSERT_INTO_SORTED_8(data, newItem);
  } else {
    INSERT_INTO_SORTED_7(&data[8], newItem);
  }
}

// sorted size is 15 before call and 16 after
// data[15] has no data before call
#define INSERT_INTO_SORTED_16  SORT_MAKE_STR(insert_into_sorted_16)
static __inline void INSERT_INTO_SORTED_16(SORT_TYPE *data, SORT_TYPE newItem) {
  SORT_TYPE item_7 = data[7];
  if (SORT_CMP(item_7, newItem) > 0) {
    data[15] = data[14];
    data[14] = data[13];
    data[13] = data[12];
    data[12] = data[11];
    data[11] = data[10];
    data[10] = data[9];
    data[9] = data[8];
    data[8] = item_7;
    INSERT_INTO_SORTED_8(data, newItem);
  } else {
    INSERT_INTO_SORTED_8(&data[8], newItem);
  }
}

#define INSERT_SORT_1  SORT_MAKE_STR(insert_sort_1)
static __inline void INSERT_SORT_1(SORT_TYPE *data) {
  return;
}

#define INSERT_SORT_2  SORT_MAKE_STR(insert_sort_2)
static __inline void INSERT_SORT_2(SORT_TYPE *data) {
  // INSERT_SORT_1(data);
  INSERT_INTO_SORTED_2(data, data[1]);
}

#define INSERT_SORT_3  SORT_MAKE_STR(insert_sort_3)
static __inline void INSERT_SORT_3(SORT_TYPE *data) {
  INSERT_SORT_2(data);
  INSERT_INTO_SORTED_3(data, data[2]);
}

#define INSERT_SORT_4  SORT_MAKE_STR(insert_sort_4)
static __inline void INSERT_SORT_4(SORT_TYPE *data) {
  INSERT_SORT_3(data);
  INSERT_INTO_SORTED_4(data, data[3]);
}

#define INSERT_SORT_5  SORT_MAKE_STR(insert_sort_5)
static __inline void INSERT_SORT_5(SORT_TYPE *data) {
  INSERT_SORT_4(data);
  INSERT_INTO_SORTED_5(data, data[4]);
}

#define INSERT_SORT_6  SORT_MAKE_STR(insert_sort_6)
static __inline void INSERT_SORT_6(SORT_TYPE *data) {
  INSERT_SORT_5(data);
  INSERT_INTO_SORTED_6(data, data[5]);
}

#define INSERT_SORT_7  SORT_MAKE_STR(insert_sort_7)
static __inline void INSERT_SORT_7(SORT_TYPE *data) {
  INSERT_SORT_6(data);
  INSERT_INTO_SORTED_7(data, data[6]);
}

#define INSERT_SORT_8  SORT_MAKE_STR(insert_sort_8)
static __inline void INSERT_SORT_8(SORT_TYPE *data) {
  INSERT_SORT_7(data);
  INSERT_INTO_SORTED_8(data, data[7]);
}

#define INSERT_SORT_9  SORT_MAKE_STR(insert_sort_9)
static __inline void INSERT_SORT_9(SORT_TYPE *data) {
  INSERT_SORT_8(data);
  INSERT_INTO_SORTED_9(data, data[8]);
}

#define INSERT_SORT_10  SORT_MAKE_STR(insert_sort_10)
static __inline void INSERT_SORT_10(SORT_TYPE *data) {
  INSERT_SORT_9(data);
  INSERT_INTO_SORTED_10(data, data[9]);
}

#define INSERT_SORT_11  SORT_MAKE_STR(insert_sort_11)
static __inline void INSERT_SORT_11(SORT_TYPE *data) {
  INSERT_SORT_10(data);
  INSERT_INTO_SORTED_11(data, data[10]);
}

#define INSERT_SORT_12  SORT_MAKE_STR(insert_sort_12)
static __inline void INSERT_SORT_12(SORT_TYPE *data) {
  INSERT_SORT_11(data);
  INSERT_INTO_SORTED_12(data, data[11]);
}

#define INSERT_SORT_13  SORT_MAKE_STR(insert_sort_13)
static __inline void INSERT_SORT_13(SORT_TYPE *data) {
  INSERT_SORT_12(data);
  INSERT_INTO_SORTED_13(data, data[12]);
}

#define INSERT_SORT_14  SORT_MAKE_STR(insert_sort_14)
static __inline void INSERT_SORT_14(SORT_TYPE *data) {
  INSERT_SORT_13(data);
  INSERT_INTO_SORTED_14(data, data[13]);
}

#define INSERT_SORT_15  SORT_MAKE_STR(insert_sort_15)
static __inline void INSERT_SORT_15(SORT_TYPE *data) {
  INSERT_SORT_14(data);
  INSERT_INTO_SORTED_15(data, data[14]);
}

#define INSERT_SORT_16  SORT_MAKE_STR(insert_sort_16)
static __inline void INSERT_SORT_16(SORT_TYPE *data) {
  INSERT_SORT_15(data);
  INSERT_INTO_SORTED_16(data, data[15]);
}

#define INSERT_SORT  SORT_MAKE_STR(insert_sort)
// it is if-else only (no loops) binary insertion sort for size <= 16
SORT_DEF void INSERT_SORT(SORT_TYPE *data, const size_t size) {
  switch (size) {
    case 0:
    case 1:
      break;

    case 2:
      INSERT_SORT_2(data);
      break;

    case 3:
      INSERT_SORT_3(data);
      break;

    case 4:
      INSERT_SORT_4(data);
      break;

    case 5:
      INSERT_SORT_5(data);
      break;

    case 6:
      INSERT_SORT_6(data);
      break;

    case 7:
      INSERT_SORT_7(data);
      break;

    case 8:
      INSERT_SORT_8(data);
      break;

    case 9:
      INSERT_SORT_9(data);
      break;

    case 10:
      INSERT_SORT_10(data);
      break;

    case 11:
      INSERT_SORT_11(data);
      break;

    case 12:
      INSERT_SORT_12(data);
      break;

    case 13:
      INSERT_SORT_13(data);
      break;

    case 14:
      INSERT_SORT_14(data);
      break;

    case 15:
      INSERT_SORT_15(data);
      break;

    case 16:
      INSERT_SORT_16(data);
      break;

    default: {
      INSERT_SORT_16(data);
      BINARY_INSERTION_SORT_START(data, 16, size);
    }
  }
}

//sorts items in chunks of the array for subsequent bottom-up merging
//chunkSize can be 2, 3 or 4
//returns actual size of sorted chunks
static __inline int STABLE_PRESORT(SORT_TYPE *data, const size_t size, size_t chunkSize) {
  size_t i;

  if (chunkSize == 2) {
    for (i = 2; i <= size; i += 2) {
      INSERT_SORT_2(&data[i - 2U]);
    }
    i -= 2;
  } else if (chunkSize == 3) {
    for (i = 3; i <= size; i += 3) {
      INSERT_SORT_3(&data[i - 3U]);
    }
    i -= 3;
  } else if (chunkSize == 4) {
    for (i = 4; i <= size; i += 4) {
      INSERT_SORT_4(&data[i - 4U]);
    }
    i -= 4;
  } else {
    return 1;
  }

  size_t remain = size - i;
  if (remain == 2) {
    INSERT_SORT_2(&data[i]);
  } else if (remain == 3) {
    INSERT_SORT_3(&data[i]);
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
#undef INSERT_INTO_SORTED_5
#undef INSERT_INTO_SORTED_6
#undef INSERT_INTO_SORTED_7
#undef INSERT_INTO_SORTED_8
#undef INSERT_INTO_SORTED_9
#undef INSERT_INTO_SORTED_10
#undef INSERT_INTO_SORTED_11
#undef INSERT_INTO_SORTED_12
#undef INSERT_INTO_SORTED_13
#undef INSERT_INTO_SORTED_14
#undef INSERT_INTO_SORTED_15
#undef INSERT_INTO_SORTED_16

#undef INSERT_SORT_1
#undef INSERT_SORT_2
#undef INSERT_SORT_3
#undef INSERT_SORT_4
#undef INSERT_SORT_5
#undef INSERT_SORT_6
#undef INSERT_SORT_7
#undef INSERT_SORT_8
#undef INSERT_SORT_9
#undef INSERT_SORT_10
#undef INSERT_SORT_11
#undef INSERT_SORT_12
#undef INSERT_SORT_13
#undef INSERT_SORT_14
#undef INSERT_SORT_15
#undef INSERT_SORT_16
#undef INSERT_SORT

#undef STABLE_PRESORT

#undef MERGE_SORT_SMALL_MERGECHUNKS
#undef MERGE_SORT_SMALL
#undef MERGE_SORT_SMALL_BALANCED
