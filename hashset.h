
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef size_t (*hashset_hash_function)(void*, void*);
typedef bool (*hashset_compare_function)(void*, void*, void*);

typedef struct hashset_class {
  hashset_hash_function hashfunction;
  hashset_compare_function comparefunction;
  void *hashfunctionarg;
  void *comparefunctionarg;
} hashset_class;

typedef struct hashset_entry {
  void *value;
  bool reserved;
} hashset_entry;

typedef struct hashset {
  hashset_class clss;
  hashset_entry *entries;
  size_t length;
} hashset;

typedef struct hashset_iterator {
  size_t index;
} hashset_iterator;

#define MAKE_LOCAL_HASHSET(varname, length, clss)\
hashset_entry __ ## varname ## _entries[(length)] = {};\
hashset __ ## varname = { { clss->hashfunction, clss->comparefunction }, __ ## varname ## _entries, (length) };\
hashset *varname = &__ ## varname;

extern const hashset_class __DEFAULT_HASHSET_CLASS;
extern const hashset_class *DEFAULT_HASHSET_CLASS;

extern void hashset_class_init (hashset_hash_function, void*, hashset_compare_function, void*, hashset_class*);
extern void hashset_init (hashset_entry*, size_t, const hashset_class*, hashset*);
extern hashset *make_hashset (size_t, const hashset_class*);
extern void free_hashset (hashset*);
extern void hashset_clear (hashset*);
extern int hashset_add (void*, hashset*);
extern int hashset_remove (void*, hashset*);
extern int hashset_get (void*, hashset*, void**);
extern bool hashset_has (void*, hashset*);
extern int hashset_count (hashset*);
extern int hashset_stretch (hashset_entry*, size_t, hashset*);
extern hashset_iterator hashset_iterate (hashset*);
extern bool hashset_iterator_has_next (hashset_iterator*, hashset*);
extern int hashset_iterator_next (hashset_iterator*, hashset*, void**);
