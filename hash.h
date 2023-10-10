#pragma once
#include "linked_list.h"
#include "extended.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>


typedef struct hash_table ioopm_hash_table_t;

//BASIC of hash table
/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *
ioopm_hash_table_create(hashing_func hh_received, compare_func cf_received,
                        size_t hash_siz);

/// @brief Delete a hash table and free its memory. 
/// This should do following: destroy iterators, destroy lists in buckets
/// and finally destroy the hash structure
/// @param ht a hash table to be deleted
void 
ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht, can insert previous
/// to some node based on compare function logic
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
option_t
ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the value mapped to by key and whether it succeded
option_t 
ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key and whether it succeeded
option_t 
ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);


///UTILS


/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t 
ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool 
ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all lists
/// @param h hash table operated upon
void 
ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param h hash table operated upon
/// @return a list of keys for hash table h
ioopm_list_t *
ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an list of values for hash table h
ioopm_list_t *
ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool 
ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool 
ioopm_hash_table_has_value(ioopm_hash_table_t *ht, 
                           elem_t value, ioopm_predicate_list pred);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool 
ioopm_hash_table_all(ioopm_hash_table_t *ht, 
                     ioopm_predicate_list pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool 
ioopm_hash_table_any(ioopm_hash_table_t *ht, 
                     ioopm_predicate_list pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void 
ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, 
                              ioopm_apply_function_list apply_fun, void *arg);


void
ioopm_hash_add_cleaner(ioopm_hash_table_t *ht, ioopm_apply_function_list cleaner);

void 
ioopm_hash_apply_extended(ioopm_hash_table_t *ht, 
                               ioopm_apply_function_list fun_value, 
                               void *extra_value,
                               ioopm_apply_function_list fun_key, 
                               void *extra_key);