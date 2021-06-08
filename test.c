
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "hashset.h"

#define TEST(form)\
if (form){\
printf("success: %s at %s:%d\n", #form, __FILE__, __LINE__);\
}\
else {\
printf("error: %s at %s:%d\n", #form, __FILE__, __LINE__); abort();\
}

static void test_hashset_add (){
  // set and get.
  {
    MAKE_LOCAL_HASHSET(hset, 128, DEFAULT_HASHSET_CLASS);
    TEST(hashset_count(hset) == 0);
    TEST(hashset_add((void*)1, hset) == 0);
    TEST(hashset_add((void*)2, hset) == 0);
    TEST(hashset_add((void*)3, hset) == 0);
    TEST(hashset_count(hset) == 3);
    void *value;
    TEST(hashset_has((void*)1, hset) == true);
    TEST(hashset_get((void*)1, hset, &value) == 0);
    TEST(value == (void*)1);
    TEST(hashset_has((void*)2, hset) == true);
    TEST(hashset_get((void*)2, hset, &value) == 0);
    TEST(value == (void*)2);
    TEST(hashset_has((void*)3, hset) == true);
    TEST(hashset_get((void*)3, hset, &value) == 0);
    TEST(value == (void*)3);
    TEST(hashset_has((void*)4, hset) == false);
    TEST(hashset_get((void*)4, hset, &value) != 0); //get unset entry (always error!)
    hashset_clear(hset);
    TEST(hashset_count(hset) == 0);
  }
}

static void test_hashset_iterate (){
  // set and iterate.
  {
    MAKE_LOCAL_HASHSET(hset, 128, DEFAULT_HASHSET_CLASS);
    TEST(hashset_count(hset) == 0);
    TEST(hashset_add((void*)1, hset) == 0);
    TEST(hashset_add((void*)2, hset) == 0);
    TEST(hashset_add((void*)3, hset) == 0);
    TEST(hashset_count(hset) == 3);
    // iterate entries.
    {
      struct hashset_iterator iterator = hashset_iterate(hset);
      void *value;
      TEST(hashset_iterator_has_next(&iterator, hset) == true);
      TEST(hashset_iterator_next(&iterator, hset, &value) == 0);
      TEST(value == (void*)1 || value == (void*)2 || value == (void*)3);
      TEST(hashset_iterator_has_next(&iterator, hset) == true);
      TEST(hashset_iterator_next(&iterator, hset, &value) == 0);
      TEST(value == (void*)1 || value == (void*)2 || value == (void*)3);
      TEST(hashset_iterator_has_next(&iterator, hset) == true);
      TEST(hashset_iterator_next(&iterator, hset, &value) == 0);
      TEST(value == (void*)1 || value == (void*)2 || value == (void*)3);
      TEST(hashset_iterator_has_next(&iterator, hset) == false);
      TEST(hashset_iterator_next(&iterator, hset, &value) != 0);
    }
  }
}

static void test_hashset_stretch (){
  // stretch hashset.
  {
    MAKE_LOCAL_HASHSET(hset, 3, DEFAULT_HASHSET_CLASS);
    TEST(hashset_add((void*)1, hset) == 0);
    TEST(hashset_add((void*)2, hset) == 0);
    TEST(hashset_add((void*)3, hset) == 0);
    TEST(hashset_add((void*)4, hset) != 0); //set to full hashset (always error!)
    struct hashset_entry newentries[128] = {};
    TEST(hashset_stretch(newentries, 128, hset) == 0);
    TEST(hashset_add((void*)4, hset) == 0);
    TEST(hashset_add((void*)5, hset) == 0);
    TEST(hashset_add((void*)6, hset) == 0);
    void *value;
    TEST(hashset_get((void*)1, hset, &value) == 0);
    TEST(value == (void*)1);
    TEST(hashset_get((void*)2, hset, &value) == 0);
    TEST(value == (void*)2);
    TEST(hashset_get((void*)3, hset, &value) == 0);
    TEST(value == (void*)3);
    TEST(hashset_get((void*)4, hset, &value) == 0);
    TEST(value == (void*)4);
    TEST(hashset_get((void*)5, hset, &value) == 0);
    TEST(value == (void*)5);
    TEST(hashset_get((void*)6, hset, &value) == 0);
    TEST(value == (void*)6);
    struct hashset_entry newentries2[3] = {};
    TEST(hashset_stretch(newentries2, 3, hset) != 0); //stretch to smaller size (always error!)
  }
}

static void test_make_hashset (){
  {
    hashset *hset = make_hashset(128, DEFAULT_HASHSET_CLASS);
    TEST(hset != NULL);
    TEST(hashset_count(hset) == 0);
    TEST(hashset_add((void*)1, hset) == 0);
    TEST(hashset_add((void*)2, hset) == 0);
    TEST(hashset_add((void*)3, hset) == 0);
    TEST(hashset_count(hset) == 3);
    void *value;
    TEST(hashset_has((void*)1, hset) == true);
    TEST(hashset_get((void*)1, hset, &value) == 0);
    TEST(value == (void*)1);
    TEST(hashset_has((void*)2, hset) == true);
    TEST(hashset_get((void*)2, hset, &value) == 0);
    TEST(value == (void*)2);
    TEST(hashset_has((void*)3, hset) == true);
    TEST(hashset_get((void*)3, hset, &value) == 0);
    TEST(value == (void*)3);
    TEST(hashset_has((void*)4, hset) == false);
    TEST(hashset_get((void*)4, hset, &value) != 0); //get unset entry (always error!)
    hashset_clear(hset);
    TEST(hashset_count(hset) == 0);
    free_hashset(hset);
  }
}

int main (){
  test_hashset_add();
  test_hashset_iterate();
  test_hashset_stretch();
  test_make_hashset();
  return 0;
}
