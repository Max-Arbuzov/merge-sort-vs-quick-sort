#define STABLE_PRESORT             SORT_MAKE_STR(stable_presort)
#define MERGE_SORT_SMALL           SORT_MAKE_STR(merge_sort_small)
#define MERGE_SORT_SMALL_BALANCED  SORT_MAKE_STR(merge_sort_small_balanced)

/*********************\
 * Merging functions *
\*********************/

// standard merge arrays
// merges two adjacent parts of the array
#define MERGE_ARRAY_STD  SORT_MAKE_STR(merge_array_std)
static __inline void MERGE_ARRAY_STD(SORT_TYPE *dest, SORT_TYPE *source, size_t sizeLeft, size_t size) {
  size_t i = 0;
  size_t j = sizeLeft;
  size_t out = 0;

  while ((i < sizeLeft) && (j < size)) {
    if (SORT_CMP(source[i], source[j]) > 0) {
      dest[out] = source[j++];
    } else {
      dest[out] = source[i++];
    }
    ++out;
  }

  while (i < sizeLeft) {
    dest[out++] = source[i++];
  }
  while (j < size) {
    dest[out++] = source[j++];
  }
}

#define MERGE_CHUNK_INPLACE  SORT_MAKE_STR(merge_chunk_inplace)
// merges two adjacent chunks into one
// i - left start
// j - right start
// end - start of the next chunk that should not be processed here
// O(N*N) item moves
static __inline void MERGE_CHUNK_INPLACE(SORT_TYPE *data, size_t i, size_t j, size_t end) {
  while ((i < j) && (j < end)) {
    SORT_TYPE item = data[j];
    if (SORT_CMP(data[i], item) > 0) {
      for (int k = j; k > i; k--) {
        data[k] = data[k - 1];
      }
      data[i] = item;
      ++j;
    }
    ++i;
  }
}

// merges (1 item) + (1 item) - > 2
#define MERGE_ITEMS_1_1  SORT_MAKE_STR(merge_items_1_1)
static __inline void MERGE_ITEMS_1_1(SORT_TYPE *dest, SORT_TYPE left_0, SORT_TYPE right_0) {
  if (SORT_CMP(left_0, right_0) > 0) {
    dest[0] = right_0;
    dest[1] = left_0;
  } else {
    dest[0] = left_0;
    dest[1] = right_0;
  }
}

// merges (1 item) + (2 items) - > 3
#define MERGE_ITEMS_1_2  SORT_MAKE_STR(merge_items_1_2)
static __inline void MERGE_ITEMS_1_2(SORT_TYPE *dest, SORT_TYPE left_0, SORT_TYPE right_0, SORT_TYPE right_1) {
  if (SORT_CMP(left_0, right_0) > 0) {
    dest[0] = right_0;
    MERGE_ITEMS_1_1(&dest[1], left_0, right_1);
  } else {
    dest[0] = left_0;
    dest[1] = right_0;
    dest[2] = right_1;
  }
}

// merges (2 items) + (1 item) - > 3
#define MERGE_ITEMS_2_1  SORT_MAKE_STR(merge_items_2_1)
static __inline void MERGE_ITEMS_2_1(SORT_TYPE *dest, SORT_TYPE left_0, SORT_TYPE left_1, SORT_TYPE right_0) {
  if (SORT_CMP(left_0, right_0) > 0) {
    dest[0] = right_0;
    dest[1] = left_0;
    dest[2] = left_1;
  } else {
    dest[0] = left_0;
    MERGE_ITEMS_1_1(&dest[1], left_1, right_0);
  }
}

// merges (1 item) + (3 items) - > 4
#define MERGE_ITEMS_1_3  SORT_MAKE_STR(merge_items_1_3)
static __inline void MERGE_ITEMS_1_3(SORT_TYPE *dest, SORT_TYPE left_0, SORT_TYPE right_0, SORT_TYPE right_1, SORT_TYPE right_2) {
  if (SORT_CMP(left_0, right_0) > 0) {
    dest[0] = right_0;
    MERGE_ITEMS_1_2(&dest[1], left_0, right_1, right_2);
  } else {
    dest[0] = left_0;
    dest[1] = right_0;
    dest[2] = right_1;
    dest[3] = right_2;
  }
}

// merges (2 items) + (2 items) - > 4
#define MERGE_ITEMS_2_2  SORT_MAKE_STR(merge_items_2_2)
static __inline void MERGE_ITEMS_2_2(SORT_TYPE *dest, SORT_TYPE left_0, SORT_TYPE left_1, SORT_TYPE right_0, SORT_TYPE right_1) {
  if (SORT_CMP(left_0, right_0) > 0) {
    dest[0] = right_0;
    MERGE_ITEMS_2_1(&dest[1], left_0, left_1, right_1);
  } else {
    dest[0] = left_0;
    MERGE_ITEMS_1_2(&dest[1], left_1, right_0, right_1);
  }
}

// merges (3 items) + (1 item) - > 4
#define MERGE_ITEMS_3_1  SORT_MAKE_STR(merge_items_3_1)
static __inline void MERGE_ITEMS_3_1(SORT_TYPE *dest, SORT_TYPE left_0, SORT_TYPE left_1, SORT_TYPE left_2, SORT_TYPE right_0) {
  if (SORT_CMP(left_0, right_0) > 0) {
    dest[0] = right_0;
    dest[1] = left_0;
    dest[2] = left_1;
    dest[3] = left_2;
  } else {
    dest[0] = left_0;
    MERGE_ITEMS_2_1(&dest[1], left_1, left_2, right_0);
  }
}

// merges 4 + (1 item) -> 5
#define MERGE_4_ITEM  SORT_MAKE_STR(merge_4_item)
static __inline void MERGE_4_ITEM(SORT_TYPE *dest, SORT_TYPE *left, SORT_TYPE right_0) {
  SORT_TYPE left_0 = left[0];
  if (SORT_CMP(left_0, right_0) > 0) {
    dest[0] = right_0;
    dest[1] = left_0;
    dest[2] = left[1];
    dest[3] = left[2];
    dest[4] = left[3];
  } else {
    dest[0] = left_0;
    MERGE_ITEMS_3_1(&dest[1], left[1], left[2], left[3], right_0);
  }
}

// merges not adjacent 3 + 2 -> 5
#define MERGE_NA_3_2  SORT_MAKE_STR(merge_na_3_2)
static __inline void MERGE_NA_3_2(SORT_TYPE *dest, SORT_TYPE *left, SORT_TYPE *right) {
  SORT_TYPE left_0 = left[0];
  SORT_TYPE right_0 = right[0];
  if (SORT_CMP(left_0, right_0) > 0) {
    dest[0] = right_0;
    MERGE_ITEMS_3_1(&dest[1], left_0, left[1], left[2], right[1]);
  } else {
    dest[0] = left_0;
    MERGE_ITEMS_2_2(&dest[1], left[1], left[2], right_0, right[1]);
  }
}

// merges not adjacent 4 + 2 -> 6
#define MERGE_NA_4_2  SORT_MAKE_STR(merge_na_4_2)
static __inline void MERGE_NA_4_2(SORT_TYPE *dest, SORT_TYPE *left, SORT_TYPE *right) {
  SORT_TYPE left_0 = left[0];
  SORT_TYPE right_0 = right[0];
  if (SORT_CMP(left_0, right_0) > 0) {
    dest[0] = right_0;
    MERGE_4_ITEM(&dest[1], left, right[1]);
  } else {
    dest[0] = left_0;
    MERGE_NA_3_2(&dest[1], &left[1], right);
  }
}

// merges adjacent 2 + 3 -> 5
#define MERGE_2_3  SORT_MAKE_STR(merge_2_3)
static __inline void MERGE_2_3(SORT_TYPE *dest, SORT_TYPE *source) {
  SORT_TYPE item_0 = source[0];
  SORT_TYPE item_2 = source[2];
  if (SORT_CMP(item_0, item_2) > 0) {
    dest[0] = item_2;
    MERGE_ITEMS_2_2(&dest[1], item_0, source[1], source[3], source[4]);
  } else {
    dest[0] = item_0;
    MERGE_ITEMS_1_3(&dest[1], source[1], item_2, source[3], source[4]);
  }
}

// merges adjacent 3 + 2 -> 5
#define MERGE_3_2  SORT_MAKE_STR(merge_3_2)
static __inline void MERGE_3_2(SORT_TYPE *dest, SORT_TYPE *source) {
  SORT_TYPE item_0 = source[0];
  SORT_TYPE item_3 = source[3];
  if (SORT_CMP(item_0, item_3) > 0) {
    dest[0] = item_3;
    MERGE_ITEMS_3_1(&dest[1], item_0, source[1], source[2], source[4]);
  } else {
    dest[0] = item_0;
    MERGE_ITEMS_2_2(&dest[1], source[1], source[2], item_3, source[4]);
  }
}

// merges adjacent 3 + 3 -> 6
#define MERGE_3_3  SORT_MAKE_STR(merge_3_3)
static __inline void MERGE_3_3(SORT_TYPE *dest, SORT_TYPE *source) {
  SORT_TYPE item_0 = source[0];
  SORT_TYPE item_3 = source[3];
  if (SORT_CMP(item_0, item_3) > 0) {
    dest[0] = item_3;
    MERGE_NA_3_2(&dest[1], source, &source[4]);
  } else {
    dest[0] = item_0;
    MERGE_2_3(&dest[1], &source[1]);
  }
}

// merges adjacent 4 + 3 -> 7
#define MERGE_4_3  SORT_MAKE_STR(merge_4_3)
static __inline void MERGE_4_3(SORT_TYPE *dest, SORT_TYPE *source) {
  SORT_TYPE item_0 = source[0];
  SORT_TYPE item_4 = source[4];
  if (SORT_CMP(item_0, item_4) > 0) {
    dest[0] = item_4;
    MERGE_NA_4_2(&dest[1], source, &source[5]);
  } else {
    dest[0] = item_0;
    MERGE_3_3(&dest[1], &source[1]);
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

/**********************************************************************\
 * Nano sorts                                                         *
 * "Nano" means the functions do not access the array while sorting   *
 * Do not contain loops                                               *
 * e.g. NANO_SORT_4 makes only 4 reads and 4 writes from/to the array *
\**********************************************************************/

// "dest" and "source" can be the same
#define NANO_SORT_MOVE_2  SORT_MAKE_STR(nano_sort_move_2)
static __inline void NANO_SORT_MOVE_2(SORT_TYPE *dest, SORT_TYPE *source) {
  MERGE_ITEMS_1_1(dest, source[0], source[1]);
}

// "dest" and "source" can be the same
#define NANO_SORT_MOVE_3  SORT_MAKE_STR(nano_sort_move_3)
static __inline void NANO_SORT_MOVE_3(SORT_TYPE *dest, SORT_TYPE *source) {
  SORT_TYPE item_0 = source[0];
  SORT_TYPE item_1 = source[1];
  SORT_TYPE item_2 = source[2];

  if (SORT_CMP(item_0, item_1) > 0) {
    MERGE_ITEMS_2_1(dest, item_1, item_0, item_2);
  } else {
    MERGE_ITEMS_2_1(dest, item_0, item_1, item_2);
  }
}

// "dest" and "source" can be the same
#define NANO_SORT_MOVE_4  SORT_MAKE_STR(nano_sort_move_4)
static __inline void NANO_SORT_MOVE_4(SORT_TYPE *dest, SORT_TYPE *source) {
  SORT_TYPE item_0 = source[0];
  SORT_TYPE item_1 = source[1];
  SORT_TYPE item_2 = source[2];
  SORT_TYPE item_3 = source[3];

  if (SORT_CMP(item_0, item_1) > 0) {
    if (SORT_CMP(item_2, item_3) > 0) {
      MERGE_ITEMS_2_2(dest, item_1, item_0, item_3, item_2);
    } else {
      MERGE_ITEMS_2_2(dest, item_1, item_0, item_2, item_3);
    }
  } else {
    if (SORT_CMP(item_2, item_3) > 0) {
      MERGE_ITEMS_2_2(dest, item_0, item_1, item_3, item_2);
    } else {
      MERGE_ITEMS_2_2(dest, item_0, item_1, item_2, item_3);
    }
  }
}

#define NANO_SORT_2  SORT_MAKE_STR(nano_sort_2)
static __inline void NANO_SORT_2(SORT_TYPE *data) {
  SORT_TYPE item_0 = data[0];
  SORT_TYPE item_1 = data[1];
  if (SORT_CMP(item_0, item_1) > 0) {
    data[0] = item_1;
    data[1] = item_0;
  }
}

// too complex, for demo only
#define NANO_SORT_3_DEMO  SORT_MAKE_STR(nano_sort_3_demo)
static __inline void NANO_SORT_3_DEMO(SORT_TYPE *data) {
  SORT_TYPE item_0 = data[0];
  SORT_TYPE item_1 = data[1];
  SORT_TYPE item_2 = data[2];
  if (SORT_CMP(item_0, item_1) > 0) {
    // item_0 > item_1 here
    if (SORT_CMP(item_0, item_2) > 0) {
      // item_0 > item_1, item_2 here
      data[2] = item_0;
      if (SORT_CMP(item_1, item_2) > 0) {
        data[0] = item_2;
      //data[1] = item_1;
      } else {
        data[0] = item_1;
        data[1] = item_2;
      }
    } else {
      // item_2 >= item_0 > item_1 here
      data[0] = item_1;
      data[1] = item_0;
    //data[2] = item_2;
    }
  } else {
    // item_0 <= item_1 here
    if (SORT_CMP(item_0, item_2) > 0) {
      // item_2 < item_0 <= item_1 here
      data[0] = item_2;
      data[1] = item_0;
      data[2] = item_1;
    } else {
      // item_0 <= item_1, item_2 here
    //data[0] = item_0;
      if (SORT_CMP(item_1, item_2) > 0) {
        data[1] = item_2;
        data[2] = item_1;
      } else {
      //data[1] = item_1;
      //data[2] = item_2;
      }
    }
  }
}

#define NANO_SORT_3  SORT_MAKE_STR(nano_sort_3)
static __inline void NANO_SORT_3(SORT_TYPE *data) {
  NANO_SORT_MOVE_3(data, data);
}

#define NANO_SORT_4  SORT_MAKE_STR(nano_sort_4)
static __inline void NANO_SORT_4(SORT_TYPE *data) {
  NANO_SORT_MOVE_4(data, data);
}

/****************************************************\
 * Micro sorts                                      *
 * "Micro" means the functions do not contain loops *
\****************************************************/

#define MICRO_MERGE_SORT_5  SORT_MAKE_STR(micro_merge_sort_5)
static __inline void MICRO_MERGE_SORT_5(SORT_TYPE *data, SORT_TYPE *temp) {
  NANO_SORT_MOVE_3(temp, data);
  NANO_SORT_MOVE_2(&temp[3], &data[3]);

  MERGE_3_2(data, temp);
}

#define MICRO_MERGE_SORT_6  SORT_MAKE_STR(micro_merge_sort_6)
static __inline void MICRO_MERGE_SORT_6(SORT_TYPE *data, SORT_TYPE *temp) {
  NANO_SORT_MOVE_3(temp, data);
  NANO_SORT_MOVE_3(&temp[3], &data[3]);

  MERGE_3_3(data, temp);
}

#define MICRO_MERGE_SORT_7  SORT_MAKE_STR(micro_merge_sort_7)
static __inline void MICRO_MERGE_SORT_7(SORT_TYPE *data, SORT_TYPE *temp) {
  NANO_SORT_MOVE_4(temp, data);
  NANO_SORT_MOVE_3(&temp[4], &data[4]);

  MERGE_4_3(data, temp);
}

/**********************************************************************\
 * Small sorts                                                        *
 * "Small" means the functions are designed for a specific array size *
 * Contain loops                                                      *
\**********************************************************************/

#define SMALL_MERGE_SORT_6  SORT_MAKE_STR(small_merge_sort_6)
static __inline void SMALL_MERGE_SORT_6(SORT_TYPE *data, SORT_TYPE *temp) {
  NANO_SORT_MOVE_3(temp, data);
  NANO_SORT_MOVE_3(&temp[3], &data[3]);

  MERGE_ARRAY_STD(data, temp, 3, 6);
}

// similar to SMALL_MERGE_SORT but contains less loops
#define SMALL_MERGE_SORT_LESSLOOPS  SORT_MAKE_STR(small_merge_sort_lessloops)
SORT_DEF void SMALL_MERGE_SORT_LESSLOOPS(SORT_TYPE *data, SORT_TYPE *temp, const size_t size) {
  switch (size) {
    case 0:
    case 1:
      break;

    case 2:
      NANO_SORT_2(data);
      break;

    case 3:
      NANO_SORT_3(data);
      break;

    case 4:
      NANO_SORT_4(data);
      break;

    case 5:
      MICRO_MERGE_SORT_5(data, temp);
      break;

    case 6:
      MICRO_MERGE_SORT_6(data, temp);
      break;

    case 7:
      MICRO_MERGE_SORT_7(data, temp);
      break;

    default: {
      MICRO_MERGE_SORT_7(data, temp);
      BINARY_INSERTION_SORT_START(data, 7, size);
    }
  }
}

// similar to SMALL_MERGE_SORT_LESSLOOPS but contains more loops
#define SMALL_MERGE_SORT  SORT_MAKE_STR(small_merge_sort)
// Stable
// O(N*logN) for size <= 6
SORT_DEF void SMALL_MERGE_SORT(SORT_TYPE *data, SORT_TYPE *temp, const size_t size) {
  switch (size) {
    case 0:
    case 1:
      break;

    case 2:
      NANO_SORT_2(data);
      break;

    case 3:
      NANO_SORT_3(data);
      break;

    case 4:
      NANO_SORT_4(data);
      break;

    case 5:
      MICRO_MERGE_SORT_5(data, temp);
      break;

    case 6:
      SMALL_MERGE_SORT_6(data, temp);
      break;

    default: {
      SMALL_MERGE_SORT_6(data, temp);
      BINARY_INSERTION_SORT_START(data, 6, size);
    }
  }
}

// size should be <= 16
#define SMALL_MERGE_SORT_LESSLOOPS_WRAP  SORT_MAKE_STR(small_merge_sort_lessloops_wrap)
SORT_DEF void SMALL_MERGE_SORT_LESSLOOPS_WRAP(SORT_TYPE *data, const size_t size) {
  SORT_TYPE tempBuf[16];

  if (size <= 1) {
    return;
  }

  SMALL_MERGE_SORT_LESSLOOPS(data, tempBuf, size);
}

#define SMALL_MERGE_SORT_WRAP  SORT_MAKE_STR(small_merge_sort_wrap)
// size should be <= 16
SORT_DEF void SMALL_MERGE_SORT_WRAP(SORT_TYPE *data, const size_t size) {
  SORT_TYPE tempBuf[16];

  if (size <= 1) {
    return;
  }

  SMALL_MERGE_SORT(data, tempBuf, size);
}

#define SMALL_MERGE_SORT_WRAP2  SORT_MAKE_STR(small_merge_sort_wrap2)
SORT_DEF void SMALL_MERGE_SORT_WRAP2(SORT_TYPE *data, const size_t size) {
  SORT_TYPE *tempBuf;

  if (size <= 1) {
    return;
  }

  tempBuf = SORT_NEW_BUFFER(size);
  SMALL_MERGE_SORT(data, tempBuf, size);
  SORT_DELETE_BUFFER(tempBuf);
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

#undef MERGE_ARRAY_STD
#undef MERGE_CHUNK_INPLACE
#undef MERGE_ITEMS_1_1
#undef MERGE_ITEMS_1_2
#undef MERGE_ITEMS_2_1
#undef MERGE_ITEMS_1_3
#undef MERGE_ITEMS_2_2
#undef MERGE_ITEMS_3_1
#undef MERGE_4_ITEM
#undef MERGE_NA_3_2
#undef MERGE_NA_4_2
#undef MERGE_2_3
#undef MERGE_3_2
#undef MERGE_3_3
#undef MERGE_4_3

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

#undef NANO_SORT_MOVE_2
#undef NANO_SORT_MOVE_3
#undef NANO_SORT_MOVE_4
#undef NANO_SORT_2
#undef NANO_SORT_3_DEMO
#undef NANO_SORT_3
#undef NANO_SORT_4
#undef MICRO_MERGE_SORT_5
#undef MICRO_MERGE_SORT_6
#undef MICRO_MERGE_SORT_7
#undef SMALL_MERGE_SORT_6
#undef SMALL_MERGE_SORT_LESSLOOPS
#undef SMALL_MERGE_SORT
#undef SMALL_MERGE_SORT_LESSLOOPS_WRAP
#undef SMALL_MERGE_SORT_WRAP
#undef SMALL_MERGE_SORT_WRAP2

#undef STABLE_PRESORT

#undef MERGE_SORT_SMALL_MERGECHUNKS
#undef MERGE_SORT_SMALL
#undef MERGE_SORT_SMALL_BALANCED
