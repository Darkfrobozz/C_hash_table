#include "include/hash.h"
#include "include/iterator.h"
#include "include/nodes.h"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef bool(*handler)(ioopm_list_t *list, void **arg);

typedef void(*mutate_func)(ioopm_list_t *list, ioopm_pred_value pred, void **extra);

struct hash_table
{
    size_t hash_siz;
    size_t elements;
    ioopm_list_t **buckets;
    //should return int.
    hashing_func hh;
    compare_func cf;
    ioopm_transform_value clean_value;
    ioopm_transform_value clean_key;

    //Chunk allocation
    pre_alloc_t *smart_list_insert;
    pre_alloc_t *smart_node_insert;
};

//CALC FUNCTION
static
option_t
fetch_list_size(elem_t list, void **arg)
{
    return (option_t) {.return_value = (elem_t) ioopm_linked_list_size(list.p), 
                       .success = 1};
}

/** HANDLER
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
    if(!list)
        return true;

    ioopm_transform_value func_value = arg[0];
    ioopm_transform_value func_key = arg[2];

    ioopm_list_apply_extended(list, func_value, arg[1], func_key, arg[3]); 
    return true;
}

/** HANDLER
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
    if(!listB) 
        return true;
    //crash if no iter
    ioopm_list_t *result = *arg;
    arg++;
    bool *take_key = *arg;
    ioopm_append_lists(result, listB, *take_key);
    return true;

}

void
ioopm_hash_add_pre_alloc(ioopm_hash_table_t *hash, 
                         pre_alloc_t *list_alloc, pre_alloc_t *node_alloc)
{
    hash->smart_list_insert = list_alloc;
    hash->smart_node_insert = node_alloc;
}

/** HANDLER
 * @brief Filters, clear, can change the nodes of the list,
 * This is not used if only values of a list are to be changed
 * 
 * @param list 
 * @param arg To be used first is the order of arguments.
 * What mutate function
 * What predicate to base mutation on
 * What extra arguments for that predicate or mutate func
 * 
 * @return true 
 * @return false 
 */
static
bool
mutate_list(ioopm_list_t *list, void **arg)
{
    mutate_func func = arg[0];
    ioopm_pred_value pred = arg[1];
    func(list, pred, arg[2]);
    return true;
}


/** HANDLER
 * @brief This is used for general calculations on the lists in the
 * hash table
 * 
 * @param list 
 * @param arg In following order
 * ioopm_calc_value
 * ioopm_comb_value
 * elem_t * (THIS IS A POINTER)
 * EXTRA for ioopm_calc_value
 * @return true 
 * @return false 
 */
static
bool
calculate_list(ioopm_list_t *list, void **arg)
{
    if(!list)
        return true;
    ioopm_calc_value func = *arg;
    arg++;
    ioopm_comb_value combining_results = *arg;
    arg++;
    elem_t *current_result = *arg;
     arg++;
    elem_t send;
    send.p = list;
    option_t fetched_result = func(send, arg);
    if(fetched_result.success)
    {
        *current_result = combining_results(*current_result, fetched_result.return_value);   
    }
    return true;
}

/** APPLYING HANDLERS TO EACH LIST
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
        work = func(list_to_handle, arg);   
    }    
}

//Creation and clearing methods
ioopm_hash_table_t *
ioopm_hash_table_create(hashing_func hh_received, compare_func cf_received, 
                        size_t i_hash_siz)
{
    ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
    ht->clean_key = NULL;
    ht->clean_value = NULL;
    ht->hash_siz = i_hash_siz;
    ht->hh = hh_received;
    ht->buckets = calloc(ht->hash_siz, sizeof(ioopm_list_t *));
    ht->cf = cf_received;
    return ht;
}

//Creation of bucket and destruction of bucket
static
void
init_bucket(ioopm_hash_table_t *ht, int key)
{
    option_t result = {0};
    if(ht->smart_list_insert)
        result = ioopm_pre_alloc_get(ht->smart_list_insert);

    if(result.success)
    {
        ht->buckets[key] = result.return_value.p;
        ht->buckets[key]->pre_alloced = true;
    }
    else        
        ht->buckets[key] = ioopm_linked_list_create();

    ioopm_list_add_pre_alloc(ht->buckets[key], ht->smart_node_insert);
}


/**
 * @brief Used by mutate lists and is responsible for clearing and destroying the list
 * Needs to change pointer to null
 * 
 * @param list 
 * @param placeholder 
 * @param arg 
 */
static
void
clear_bucket(ioopm_list_t *list, ioopm_pred_value placeholder, void **arg)
{
    ioopm_hash_table_t *ht = arg[0];
    int *key = arg[1];


    if(list)
    {
        //making sure cleaners are recursive could make pointers instead
        ioopm_add_cleaners(list, ht->clean_value, ht->clean_key);

        //making sure list is destroyed
        ioopm_linked_list_clear(list);
        ioopm_linked_list_destroy(list);
        ht->buckets[*key] = NULL;
    }

    (*key)++;
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
    //this can be done else where...
    int index = 0;
    void *extra[] = {ht, &index};
    void *arg[] = {clear_bucket, NULL, extra}; 
    
    //to do this to all lists in buckets
    pipe_lists(ht, mutate_list, arg);
    for(int i = 0; i < ht->hash_siz; i++) ht->buckets[i] = NULL;
}


//navigation methods
option_t
iterate_find_key(ioopm_iterator_t *iter, compare_func cf, elem_t key)
{
    option_t result = {.success = MOVE_ON, .return_value = (elem_t) 0};
    //TODO CREATE ITERATOR INSTEAD
    if (ioopm_iter_list_siz(iter) == 0)
        return result;
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
            result.success = REPLACE;
            result.return_value = ioopm_iterator_current_value(iter).return_value;
            return result;
        }
        //this is for insertion prior to element, this also means
        //that when using lookup we only go to the first to make us insert previous
        if(command == INSERT_PREVIOUS)
        {
            //DOES NOT WORK IF WE ARE IN START SINCE WE CAN'T MOVE BACK
            result.success = INSERT_PREVIOUS;
            return result;
        }
    }
    while(ioopm_iterator_next(iter).success);
    
    return result;
}

static
int
applying_hash_func(ioopm_hash_table_t *hash, elem_t key)
{
    return hash->hh(key, &(hash->hash_siz));
}
/**
 * @brief Also creates a list if empty with method init
 * 
 * @param ht 
 * @param key 
 * @return iterator
 */
ioopm_iterator_t *
get_bucket_iter(ioopm_hash_table_t *ht, int index)
{
    //seperate into a new functions finding key!
    ioopm_list_t *list_s = ht->buckets[index];

    //if list is empty initiate the list.
    if(!list_s) 
        init_bucket(ht, index);

    //reseting list
    list_s = ht->buckets[index];

    //Making sure current cleaner of list is same as cleaners of hashtable
    //This will only be necessary if cleaners are updated though.
    ioopm_add_cleaners(list_s, ht->clean_value, ht->clean_key);
    
    ioopm_iterator_t *list_iterator = ioopm_list_iterator(list_s);

    return list_iterator;
}



option_t
ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
    option_t result = {0};
    if(!ht)
        return result;

    int hash_key = applying_hash_func(ht, key);    

    ioopm_iterator_t *iter = get_bucket_iter(ht, hash_key);
    result = iterate_find_key(iter, ht->cf, key);

    if(result.success != REPLACE)
        result.success = 0;
    
    //cleaning up iterator from get_bucket_iter
    if(iter)
        ioopm_iterator_destroy(iter);
    
    return result;
}

static
void
hash_remove_node(ioopm_iterator_t *iter, ioopm_hash_table_t *ht, bool success)
{
    if(success == REPLACE)
    {
       ioopm_iterator_remove(iter);
       ht->elements--;
    }
}

option_t 
ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{ 
    if(!ht)
        return (option_t) {0};

    int index = applying_hash_func(ht, key);
    ioopm_iterator_t *iter = get_bucket_iter(ht, index);
    option_t result = iterate_find_key(iter, ht->cf, key);


    switch (result.success)
    {
        case INSERT_PREVIOUS:
        {
            ht->elements++;
            result = ioopm_iterator_insert(iter, value, key, LEFT);
            break;
        }
        
        case REPLACE:
        {
            ioopm_iterator_edit(iter, NULL, &value);
            result = ioopm_iterator_current_value(iter);
            break;
        }
        
        case MOVE_ON:
        {
            ht->elements++;
            result = ioopm_iterator_insert(iter, value, key, RIGHT);
            break;
        }
    }

    if(iter)
        ioopm_iterator_destroy(iter);
    
    return result;

}

option_t
ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
    int index = applying_hash_func(ht, key);
    ioopm_iterator_t *iter = get_bucket_iter(ht, index);
    
    option_t to_remove = iterate_find_key(iter, ht->cf, key);
        
    hash_remove_node(iter, ht, to_remove.success);

    if(to_remove.success != REPLACE)
    {
        to_remove.success = 0; 
    }


    ioopm_iterator_destroy(iter);
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
ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_pred_value pred, 
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
                           ioopm_pred_value pred)
{
    elem_t value_storage = value;
    return ioopm_hash_table_any(ht, pred, &value_storage);
}

bool
ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_pred_value pred, 
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
                              ioopm_transform_value apply_fun, 
                              void *arg)
{

    ioopm_hash_apply_extended(ht, apply_fun, arg, NULL, NULL);
}

void
ioopm_hash_add_cleaner(ioopm_hash_table_t *ht, ioopm_transform_value i_clean_key, 
                       ioopm_transform_value i_clean_value)
{
    ht->clean_key = i_clean_key;
    ht->clean_value = i_clean_value;

}
void ioopm_hash_apply_extended(ioopm_hash_table_t *ht, 
                               ioopm_transform_value fun_value, void *extra_value,
                               ioopm_transform_value fun_key, void *extra_key)
{    
    if(!fun_value && !fun_key)
        return; 
    void *arg_send[] = {fun_value, extra_value, fun_key, extra_key};
    pipe_lists(ht, transform_elements, arg_send);
}

static
ioopm_list_t *
size_list(ioopm_hash_table_t *ht)
{
    ioopm_list_t *size_list = ioopm_linked_list_create();
    elem_t result;
    result.p = size_list;
    void *arg[] = {fetch_list_size, ioopm_element_to_list, &result, NULL};

    pipe_lists(ht, calculate_list, arg);

    return size_list;
}

static
option_t
hash_table_remove_no_clean(ioopm_hash_table_t *ht, elem_t key)
{
    // save clening functions for key and value
    void *value_cleaner = ht->clean_value;
    void *key_cleaner = ht->clean_key;
    
    // disable cleaning functions for know
    ioopm_hash_add_cleaner(ht, key_cleaner, NULL);
    
    // remove
    option_t result = ioopm_hash_table_remove(ht, key);

    // enable 
    ioopm_hash_add_cleaner(ht, key_cleaner, value_cleaner);
    return result;
}


bool
ioopm_evaluate_hash(ioopm_hash_table_t *ht)
{
    ioopm_list_t *balance = size_list(ht);
    //To achieve perfect balance we should be looking for (elements / buckets to the power of two) times buckets
    //aka a perfect spread of elements
    double amount_element = ht->elements;
    double hash_size = ht->hash_siz;
    double ideal_balance = pow((amount_element/ hash_size), 2) * hash_size;
    ioopm_linked_list_clear(balance);
    ideal_balance--;
    ioopm_linked_list_destroy(balance);
    return false;
    //Gather data
    //Balance could look for mode... In other words return highest list size
    // 

    //make decision, look for two things unbalance and 
}

option_t
ioopm_hash_bucket_iter(ioopm_hash_table_t *ht, elem_t key)
{
    ioopm_iterator_t *iter = get_bucket_iter(ht, applying_hash_func(ht, key));
    elem_t iter_element;
    iter_element.p = iter;
    option_t result = iterate_find_key(iter, ht->cf, key);
    if(result.success != REPLACE)
    {
        ioopm_iterator_destroy(iter);
        result.success = 0;
        return result;
    }
    result.return_value = iter_element;
    return result;

}

option_t
ioopm_hash_edit(ioopm_hash_table_t *ht, ioopm_transform_value edit, 
                elem_t key, void *arg)
{
    ioopm_iterator_t *iter = get_bucket_iter(ht, applying_hash_func(ht, key));
    option_t result = iterate_find_key(iter, ht->cf, key);
    
    if(result.success != REPLACE)
    {
        // if it can not find it
        ioopm_iterator_destroy(iter);
        return (option_t) {0};
    }
    
    ioopm_iterator_edit(iter, edit, arg);
    elem_t new_v = ioopm_iterator_current_value(iter).return_value;
    ioopm_iterator_destroy(iter);
    return (option_t) {.return_value = new_v, .success = 1};
}

option_t
ioopm_rehash(ioopm_hash_table_t *ht, elem_t old_key, elem_t new_key)
{
    //get content from current then remove old
    option_t result = ioopm_hash_table_lookup(ht, old_key);
    ioopm_hash_table_insert(ht, new_key, result.return_value); 
    // remove if old_key exists, returns false otherwise
    hash_table_remove_no_clean(ht, old_key);
    if(!result.success)
        return result;
    return result;     
}