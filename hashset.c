
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashset.h"

static size_t default_hash_function (void *value, void *arg){
  return (size_t)value;
}

static bool default_compare_function (void *value1, void *value2, void *arg){
  return value1 == value2;
}

const hashset_class __DEFAULT_HASHSET_CLASS = {
  default_hash_function,
  default_compare_function,
  NULL,
  NULL,
};

const hashset_class *DEFAULT_HASHSET_CLASS = &__DEFAULT_HASHSET_CLASS;

static void clear_hashset_entries (hashset_entry *entries, size_t length){
  for (size_t index = 0; index < length; index++){
    entries[index].reserved = false;
  }
}

void hashset_class_init (hashset_hash_function hashfunction, void *hashfunctionarg, hashset_compare_function comparefunction, void *comparefunctionarg, hashset_class *clss){
  clss->hashfunction = hashfunction;
  clss->comparefunction = comparefunction;
  clss->hashfunctionarg = hashfunctionarg;
  clss->comparefunctionarg = comparefunctionarg;
}

void hashset_init (hashset_entry *entries, size_t length, const hashset_class *clss, hashset *hset){
  clear_hashset_entries(entries, length); 
  hset->clss = *clss;
  hset->entries = entries;
  hset->length = length;
}

hashset *make_hashset (size_t length, const hashset_class *clss){
  hashset *hset = malloc(sizeof(hashset));
  if (hset == NULL){
    return NULL;
  }
  hashset_entry *hsetentries = malloc(length * sizeof(hashset_entry));
  if (hsetentries == NULL){
    free(hset);
    return NULL;
  }
  hashset_init(hsetentries, length, clss, hset);
  return hset;
}

void free_hashset (hashset *hset){
  free(hset->entries);
  free(hset);
}

void hashset_clear (hashset *hset){
  for (size_t index = 0; index < hset->length; index++){
    hset->entries[index].reserved = false;
  }
}

int hashset_add (void *value, hashset *hset){
  if (0 < hset->length){
    size_t hash = hset->clss.hashfunction(value, hset->clss.hashfunctionarg);
    for (size_t index = 0; index < hset->length; index++){
      size_t trueindex = (index + hash) % hset->length;
      if (hset->entries[trueindex].reserved){
        if (hset->clss.comparefunction(hset->entries[trueindex].value, value, hset->clss.comparefunctionarg)){
          hset->entries[trueindex].value = value;
          return 0;
        }
      }
      else {
        hset->entries[trueindex].value = value;
        hset->entries[trueindex].reserved = true;
        return 0;
      }
    }
    return 1;
  }
  else {
    return 1;
  }
}

int hashset_remove (void *value, hashset *hset){
  if (0 < hset->length){
    size_t hash = hset->clss.hashfunction(value, hset->clss.hashfunctionarg);
    for (size_t index = 0; index < hset->length; index++){
      size_t trueindex = (index + hash) % hset->length;
      if (hset->entries[trueindex].reserved){
        if (hset->clss.comparefunction(hset->entries[trueindex].value, value, hset->clss.comparefunctionarg)){
          hset->entries[trueindex].reserved = false;
          return 0;
        }
      }
      else {
        return 1;
      }
    }
    return 1;
  }
  else {
    return 1;
  }
}

int hashset_get (void *value, hashset *hset, void **valuep){
  if (0 < hset->length){
    size_t hash = hset->clss.hashfunction(value, hset->clss.hashfunctionarg);
    for (size_t index = 0; index < hset->length; index++){
      size_t trueindex = (index + hash) % hset->length;
      if (hset->entries[trueindex].reserved){
        if (hset->clss.comparefunction(hset->entries[trueindex].value, value, hset->clss.comparefunctionarg)){
          *valuep = hset->entries[trueindex].value;
          return 0;
        }
      }
      else {
        return 1;
      }
    }
    return 1;
  }
  else {
    return 1;
  }
}

bool hashset_has (void *value, hashset *hset){
  void *foundvalue;
  return hashset_get(value, hset, &foundvalue) == 0;
}

int hashset_count (hashset *hset){
  size_t count = 0;
  for (size_t index = 0; index < hset->length; index++){
    if (hset->entries[index].reserved){
      count += 1;
    }
  }
  return count;
}

int hashset_stretch (hashset_entry *newentries, size_t newlength, hashset *hset){
  hashset newhset;
  hashset_init(newentries, newlength, &(hset->clss), &newhset);
  for (size_t index = 0; index < hset->length; index++){
    if (hset->entries[index].reserved){
      if (hashset_add(hset->entries[index].value, &(newhset)) != 0){ return 1; }
    }
  }
  *hset = newhset;
  return 0;
}

hashset_iterator hashset_iterate (hashset *hset){
  hashset_iterator newiterator = { 0 };
  return newiterator;
}

bool hashset_iterator_has_next (hashset_iterator *iterator, hashset *hset){
  for (size_t index = iterator->index; index < hset->length; index++){
    if (hset->entries[index].reserved){
      return true;
    }
  }
  return false; 
}

int hashset_iterator_next (hashset_iterator *iterator, hashset *hset, void **valuep){
  for (size_t index = iterator->index; index < hset->length; index++){
    if (hset->entries[index].reserved){
      iterator->index = index +1;
      *valuep = hset->entries[index].value;
      return 0;
    }
  }
  return 1; 
}
