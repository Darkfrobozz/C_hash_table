#include "hash.h"
#include "iterator.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef bool(*handler)(ioopm_list_t *list, void **arg);

typedef void(*mutate_func)(ioopm_list_t *list, ioopm_predicate_list pred, void **extra);

typedef option_t(*calc_func)(ioopm_list_t *list, void **extra);
typedef elem_t(*add_func)(elem_t a, elem_t b);

struct hash_table
{
    size_t hash_siz;
    ioopm_list_t **buckets;
    hashing_func hh;
    //should return int.
    compare_func cf;
    ioopm_apply_function_list clean_key;
    ioopm_apply_function_list clean_value;
};

/**
 * @brief 
 * 
 * @param list 
 * @param arg
 * Apply function
 * Extra to apply function 
 * @return true 
 * @return false 
 */

static
bool
transform_elements(ioopm_list_t *list, void **arg)
{
    ioopm_apply_function_list func_value = arg[0];
    ioopm_apply_function_list func_key = arg[2];

    ioopm_list_apply_extended(list, func_value, arg[1], func_key, arg[3]); 
    return true;
}
/**
 * @brief 
 * 
 * @param listB 
 * @param arg 
 * Result list
 * take_key arg
 * @return true 
 * @return false 
 */
static
bool
fetch_list_elements(ioopm_list_t *listB, void **arg)
{
    if(!listB) return true;
    //crash if no iter
    ioopm_list_t *result = *arg;
    arg++;
    bool *take_key = *arg;
    ioopm_append_lists(result, listB, *take_key);
    return true;

}

/**
 * @brief Filters, clear, can change the nodes of the list,
 * This is not used if only values of a list are to be changed
 * 
 * @param list 
 * @param arg To be used first is the order of arguments.
 * What mutate function
 * What predicate to base mutation on
 * What extra arguments for that predicate or mutate func
 * @return true 
 * @return false 
 */
static
bool
mutate_list(ioopm_list_t *list, void **arg)
{
    mutate_func func = *arg;
    arg++;
    ioopm_predicate_list pred = *arg;
    arg++;
    func(list, pred, arg);
    return true;
}

static
option_t
fetch_list_size(ioopm_list_t *list, void **arg)
{
    option_t result = {0};
    if(list)
    {
        result.return_value.siz = ioopm_linked_list_size(list);
        result.success = 1;
    }
    return result;
}
/**
 * @brief This is used for general calculations on the lists in the
 * hash table
 * 
 * @param list 
 * @param arg In following order
 * Calc_func
 * add_func
 * elem_t * (THIS IS A POINTER)
 * EXTRA for calc_func
 * @return true 
 * @return false 
 */
static
bool
calculate_list(ioopm_list_t *list, void **arg)
{
    calc_func func = *arg;
    arg++;
    add_func combining_results = *arg;
    arg++;
    elem_t *current_result = *arg;
    arg++;
    option_t fetched_result = func(list, arg);
    if(fetched_result.success)
    {
        *current_result = combining_results(*current_result, fetched_result.return_value);   
    }
    return true;
}
/**
 * @brief Applies some handler to each list in buckets of some hashtable
 * 
 * @param ht hashtable
 * @param func handler that applies a function to the lists responsible for interpetting args
 * @param arg Very important to be used in conjunction with handler chosen
 */
static
void
pipe_lists(ioopm_hash_table_t *ht, handler func, void **arg)
{
    bool work = true;
    for(int index = 0; index < ht->hash_siz && work; index++)
    {
        ioopm_list_t *list_to_handle = ht->buckets[index];
        if(list_to_handle)
        {
            work = func(list_to_handle, arg);   
        }
    }
    
}

ioopm_hash_table_t *
ioopm_hash_table_create(hashing_func hh_received, compare_func cf_received, 
                        size_t hash_siz)
{
    ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
    ht->hash_siz = hash_siz;
    ht->buckets = calloc(ht->hash_siz, sizeof(ioopm_list_t *));
    ht->hh = hh_received;
    ht->cf = cf_received;
    return ht;
}

void 
ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
    free(ht->buckets);
    free(ht);
    // used in conjunction with hash table clear ASSIGNMENTSTEP2
}

void
ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
    //Clear and destroy destroys iterators, clears lists and destroys headers
    //and dummies of list
    ioopm_hash_apply_extended(ht, ht->clean_value, NULL, ht->clean_key, NULL);
    void *arg[] = {ioopm_list_clear_and_destroy, NULL, NULL}; 
    
    //to do this to all lists in buckets
    pipe_lists(ht, mutate_list, arg);
    for(int i = 0; i < ht->hash_siz; i++) ht->buckets[i] = NULL;
}

option_t
iterate_find_key(ioopm_iterator_t *iter, compare_func cf, elem_t key)
{
    option_t result = {0};
    //TODO CREATE ITERATOR INSTEAD
    ioopm_iterator_reset(iter);
    do
    {
        option_t current_key = ioopm_iterator_current_key(iter);
        
        if(!current_key.success)
            continue;
        
        int command = cf(current_key.return_value, key);

        if(command == REPLACE)
        {
            //This does not work either at first block, if we try to insert after deleting
            //we will be inserting after the element/order will be wrong
            result.success = 1;
            return result;
        }
        //this is for insertion prior to element, this also means
        //that when using lookup we only go to the first to make us insert previous
        if(command == INSERT_PREVIOUS)
        {
            //DOES NOT WORK IF WE ARE IN START SINCE WE CAN'T MOVE BACK
            return result;
        }
    }
    while(ioopm_iterator_next(iter).success);
    return result;
}

void
init_bucket(ioopm_hash_table_t *ht, int key)
{
    ht->buckets[key] = ioopm_linked_list_create();
    ioopm_list_iterator(ht->buckets[key]);
}
/**
 * @brief Also creates a list if empty with method init
 * 
 * @param ht 
 * @param key 
 * @return iterator
 */
ioopm_iterator_t *
get_bucket_iter(ioopm_hash_table_t *ht, elem_t key)
{
    int buckets = (int) ht->hash_siz;
    int index = ht->hh(key, &buckets);
    ioopm_list_t *list_s = ht->buckets[index];

    //if list is empty initiate the list.
    if(!list_s) init_bucket(ht, index);
    list_s = ht->buckets[index];

    ioopm_list_t *list_iterator = ioopm_get_iterator(list_s);

    ioopm_iterator_t *current =
    ioopm_linked_list_get(list_iterator, 0).return_value.p;

    return current;
}



option_t
ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
    option_t result = {0};
    if(!ht)
        return result;
    return iterate_find_key(get_bucket_iter(ht, key), ht->cf, key);
}

void
hash_remove_node(ioopm_iterator_t *iter, ioopm_hash_table_t *ht, bool success)
{
    if(success)
    {
        if(ht->clean_key)
        {
            elem_t key = ioopm_iterator_current_key(iter).return_value;
            ht->clean_key(&key, NULL);
        }
        if(ht->clean_value)
        {
            elem_t value = ioopm_iterator_current_value(iter).return_value;
            ht->clean_value(&value, NULL);
        }
        ioopm_iterator_remove(iter);
    }
}

option_t 
ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{ 
    if(!ht)
        return (option_t) {0};

    ioopm_iterator_t *iter = get_bucket_iter(ht, key);
    option_t result = iterate_find_key(iter, ht->cf, key);

    ioopm_iterator_insert(iter, value, key, RIGHT);

    if(result.success)
    {
        ioopm_iterator_previous(iter);
        hash_remove_node(iter, ht, result.success);
    }

    return result;

}

option_t
ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
    ioopm_iterator_t *iter = get_bucket_iter(ht, key);
    
    option_t to_remove = iterate_find_key(iter, ht->cf, key);
    
    
    hash_remove_node(iter, ht, to_remove.success);

    return to_remove;
}

size_t
ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
    elem_t result;
    result.siz = 0;
    void *arg[] = {fetch_list_size, add_siz, &result, NULL};
    pipe_lists(ht, calculate_list, arg);

    return result.siz;
}

bool
ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
    return ioopm_hash_table_size(ht) == 0;
}

ioopm_list_t *
ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
    ioopm_list_t *list = ioopm_linked_list_create();
    bool take_keys = true;
    void *arg[] = {list, &take_keys};
    pipe_lists(ht, fetch_list_elements, arg);
    return list;
}

ioopm_list_t *
ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
    ioopm_list_t *list = ioopm_linked_list_create();
    bool take_keys = false;
    void *arg[] = {list, &take_keys};
    pipe_lists(ht, fetch_list_elements, arg);
    return list;
}

bool
ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
    return ioopm_hash_table_lookup(ht, key).success;
}


bool
ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate_list pred, 
                     void *arg)
{
    ioopm_list_t *values = ioopm_hash_table_values(ht);
    bool result = ioopm_linked_list_any(values, pred, arg);
    ioopm_linked_list_clear(values);
    ioopm_linked_list_destroy(values);
    return result;

}

bool
ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value, 
                           ioopm_predicate_list pred)
{
    elem_t value_storage = value;
    return ioopm_hash_table_any(ht, pred, &value_storage);
}

bool
ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate_list pred, 
                     void *arg)
{
    ioopm_list_t *values = ioopm_hash_table_values(ht);
    bool result = ioopm_linked_list_all(values, pred, arg);
    ioopm_linked_list_clear(values);
    ioopm_linked_list_destroy(values);
    return result;
}

void
ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, 
                              ioopm_apply_function_list apply_fun, 
                              void *arg)
{

    ioopm_hash_apply_extended(ht, apply_fun, arg, NULL, NULL);
}

void
ioopm_hash_add_cleaner(ioopm_hash_table_t *ht, ioopm_apply_function_list i_clean_key, 
                       ioopm_apply_function_list i_clean_value)
{
    ht->clean_key = i_clean_key;
    ht->clean_value = i_clean_value;
}

void ioopm_hash_apply_extended(ioopm_hash_table_t *ht, 
                               ioopm_apply_function_list fun_value, void *extra_value,
                               ioopm_apply_function_list fun_key, void *extra_key)
{    
    if(!fun_value && !fun_key)
        return; 
    void *arg_send[] = {fun_value, extra_value, fun_key, extra_key};
    pipe_lists(ht, transform_elements, arg_send);
}