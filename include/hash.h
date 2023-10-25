#pragma once
#include "linked_list.h"
#include <stdio.h>


typedef struct hash_table ioopm_hash_table_t;


/// BASIC of hash table


/// @brief Create a new hash table
/// @param hh_received
/// @param cf_received
/// @param hash_siz
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
/// @return 
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
/// @param ht hash table operated upon
/// @return the number of key => value entries in the hash table
size_t 
ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param ht hash table operated upon
/// @return true is size == 0, else false
bool 
ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all lists
/// @param ht hash table operated upon
void 
ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map 
/// (in no particular order, but same as ioopm_hash_table_values).
/// @param ht hash table operated upon
/// @return a list of keys for hash table h
ioopm_list_t *
ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map 
/// (in no particular order, but same as ioopm_hash_table_keys).
/// @param ht hash table operated upon
/// @return a list of values for hash table h
ioopm_list_t *
ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought
/// @return yes or no 
bool 
ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought
/// @param pred
/// @return yes or no 
bool 
ioopm_hash_table_has_value(ioopm_hash_table_t *ht, 
                           elem_t value, ioopm_pred_value pred);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return yes or no 
bool 
ioopm_hash_table_all(ioopm_hash_table_t *ht, 
                     ioopm_pred_value pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return yes or no 
bool 
ioopm_hash_table_any(ioopm_hash_table_t *ht, 
                     ioopm_pred_value pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void 
ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, 
                              ioopm_transform_value apply_fun, void *arg);

/// @brief Adds cleaners to hash
/// @param ht hash
/// @param clean_key 
/// @param clean_value 
void
ioopm_hash_add_cleaner(ioopm_hash_table_t *ht, 
                       ioopm_transform_value clean_key,
                       ioopm_transform_value clean_value);

/// @brief 
/// @param ht 
/// @param fun_value 
/// @param extra_value 
/// @param fun_key 
/// @param extra_key
void 
ioopm_hash_apply_extended(ioopm_hash_table_t *ht, 
                               ioopm_transform_value fun_value, 
                               void *extra_value,
                               ioopm_transform_value fun_key, 
                               void *extra_key);

/// @brief Evaluates based on fill percent whether to resize
/// @param ht
/// @return yes or no 
bool
ioopm_evaluate_hash(ioopm_hash_table_t *ht);

/// @brief This is to inspect the balance of the hash
/// 
/// @return ioopm_list_t *, returns a list with size and hash id.
/// Lists that do not exist will be 0 size.
ioopm_list_t *
ioopm_get_size_table(void);

/// @brief This function edits a function
///
/// @param ht the hashtable
/// @param edit functions a transform value func
/// @param key 
/// @param arguments passed directly to the edit func
/// @return success and pointer to new merch
option_t
ioopm_hash_edit(ioopm_hash_table_t *ht, ioopm_transform_value edit, 
                elem_t key, void *arg);

/// @brief Rehashes
///
/// @param ht hashtable to rehash in
/// @param old_key Key to container to rehash
/// @param new_key Key 
/// @return option_t, contains adress to value of node 
option_t
ioopm_rehash(ioopm_hash_table_t *ht, elem_t old_key, elem_t new_key);


/**
 * @brief Getting iterator initiated at key given
 * 
 * @param ht 
 * @param key 
 * @return option_t 
 */
option_t
ioopm_hash_bucket_iter(ioopm_hash_table_t *ht, elem_t key);