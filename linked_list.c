#include "include/nodes.h"
#include "include/automaton.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//List macros
#define send_update(x,y) ioopm_linked_list_apply_to_all(x, ioopm_update_iterators, y)


typedef void(*handler)(node_t *node, bool *success, void **arg);

static
void
bind(node_t *left, node_t *right)
{
    left->next = right;
    right->previous = right;
}

static
void
append_iterator(ioopm_list_t *list, ioopm_iterator_t *iter)
{
    if(iter)
        ioopm_linked_list_append(list, 
                                 (elem_t) (void *) iter, (elem_t) NULL);
}

static
ioopm_list_t *
ioopm_iterator_list_create()
{
    ioopm_list_t *list = ioopm_linked_list_create();
    ioopm_add_cleaners(list, ioopm_iter_apply_destroy, NULL);
    return list;
}

ioopm_list_t *
bond_iter(ioopm_list_t *iter_list, ioopm_iterator_t *iter)
{
    if(!iter_list)
        iter_list = ioopm_iterator_list_create(iter);
    append_iterator(iter_list, iter);
    return iter_list;
}

ioopm_list_t *
list_track_iter(ioopm_list_t *list, ioopm_iterator_t *iter)
{
    list->iterator_list = bond_iter(list->iterator_list, iter);
    return list->iterator_list;
}

static
void
iter_send_updates(ioopm_list_t *iter_list, int index, enum updates update)
{
    int send_i = index - 1;
    enum updates send_t = update;
    void *action_info = {&send_t, &send_i};
    send_update(iter_list, action_info);
}

ioopm_list_t *
ioopm_linked_list_create(void)
{
    ioopm_list_t *result = calloc(1, sizeof(ioopm_list_t));
    node_t *first = calloc(1, sizeof(node_t));
    node_t *last = calloc(1, sizeof(node_t));

    result->first = first;
    result->last = last;
    bind(first, last);

    return result;
}

void 
ioopm_linked_list_destroy(ioopm_list_t *list)
{
    if(!list) 
        return;
    //iterator_list is merely destroyed but not cleared!
    ioopm_linked_list_clear(list->iterator_list);
    ioopm_linked_list_destroy(list->iterator_list);
    free(list->first);
    free(list->last);
    free(list);
}

/**
 * @brief Contracts list around a node
 * 
 * @param node Point to contract around 
 */
static
void
contract(node_t *node)
{    
    //selecting nodes around point
    node_t *prev_node = node->previous;
    node_t *next_node = node->next;
    //making sure nodes exist
    assert(prev_node);
    assert(next_node);
    bind(prev_node, next_node);
}


/**
 * @brief This is called when removing a node 
 * 
 * @param node_to_clean node to clean 
 * @param clean_value This will not work if cleaner is not
 * correctly configured to value
 * @param clean_key Same note as clean_value
 */
static
void
clean_data(elem_t *value, elem_t *key, 
           ioopm_transform_value clean_value, 
           ioopm_transform_value clean_key)
{
    if(clean_key)
        clean_key(&key, NULL);
    if(clean_value)
        clean_value(&value, NULL);    
}


//BIG THREE SIDE EFFECTS GIVEN A NODE

void
ioopm_list_insert(node_t *prev_node, elem_t i_value, 
                 elem_t i_key, ioopm_list_t *list, 
                 int index)
{
    node_t *next_node = prev_node->next;
    node_t *insert = calloc(1, sizeof(node_t));
    insert->value = i_value;
    insert->key = i_key;

    bind(prev_node, insert);    
    bind(insert, next_node);    
    
    iter_send_updates(list->iterator_list, index, inserted);

    list->size++;
}

void
ioopm_list_edit(ioopm_list_t *list, 
                ioopm_transform_value edit_function, 
                node_t *node_edit, void *arg)
{
    elem_t *to_edit = &(node_edit->value);
    //EDITING
    if(edit_function)
        return edit_function(to_edit, arg);

    //IN CASE NO EDIT DEFAULTS TO REPLACE

    //Here we clean up before replacing.
    void *args[] = {arg, list->clean_value};
    ioopm_replace(to_edit, args);
}

void 
ioopm_list_remove(ioopm_list_t *list, node_t *remove_node, int index)
{
    //informing iterators
    iter_send_updates(list->iterator_list, index, removed);

    //contracting, this should never be a non existent node.
    contract(remove_node);
    
    //clean the value of nodes
    clean_data(&remove_node->value, &remove_node->key, 
               list->clean_value, list->clean_key);

    //Free the node
    free(remove_node);

    //Decrease size
    list->size--;
}

//USES PREXISTING LOGIC
option_t 
ioopm_linked_list_insert(ioopm_list_t *list, int index, 
                         elem_t i_value, elem_t i_key)
{
    option_t result = {0};
    create_in(list);

    if(jump(iter, index))
    {
        insert(iter, i_value, i_key); 
        result = value(iter);
    }
    destroy(iter);
    return result;
}

option_t 
ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    option_t result = {0};
    create_in(list);
    if(jump(iter, index))
        result = value(iter);
    destroy(iter);
}

option_t 
ioopm_linked_list_remove(ioopm_list_t *list, int index)
{

    option_t result = {0};
    create_in(list);
    if(jump(iter,index))
    {
        result = value(iter);
        remove(iter);
    }
    destroy(iter);

    return result;
}

//COMFORT FUNCTIONS
option_t 
ioopm_linked_list_append(ioopm_list_t *list, elem_t i_value, elem_t i_key)
{
    return ioopm_linked_list_insert(list, list->size, 
                                    i_value, i_key);    
}


option_t 
ioopm_linked_list_prepend(ioopm_list_t *list, elem_t i_value, elem_t i_key)
{
    return ioopm_linked_list_insert(list, 0, i_value, i_key);
}

size_t 
ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool 
ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return (list->size == 0);
}

ioopm_list_t *
ioopm_get_iterator(ioopm_list_t *list)
{
    return list->iterator_list;
}


//PIPELINE FUNCTIONS NOT READY TO REPLACE
//Pipeline uses old logic so index is not a center point which is a
//huge issue

/**
 * @brief Handler function to apply a predicate function
 * 
 * @param node To apply to
 * @param success Communicate outcome of predicate
 * @param arg Contains predicate to apply and extra arguments to it
 */
void 
true_all(node_t *node, bool *success, void **arg)
{
    ioopm_pred_value pred = arg[0];
    if(!pred(node->value, arg[1]))
    {
        *success = false;
    }
}

//similar to true all
void 
true_any(node_t *node, bool *success, void **arg)
{
    ioopm_pred_value pred = arg[0];
    if(pred(node->value, arg[1]))
    {
        *success = true;
    }
}

/**
 * @brief 
 * 
 * @param node 
 * @param success 
 * @param arg
 * Takes in a list to append to
 */
void
append_node(node_t *node, bool *take_key, void **arg)
{
    ioopm_list_t *list = *arg;
    if(*take_key)
    {
        ioopm_linked_list_append(list, node->key, node->value);
        return;
    }

    ioopm_linked_list_append(list, node->value, node->key);
}

void 
transform_list_node(node_t *node, bool *success, void **arg)
{
    ioopm_transform_value func_value = arg[0];
    ioopm_transform_value func_key = arg[1];
    if(func_value)
        func_value(&(node->value), arg[2]);
    if(func_key)
        func_key(&(node->key), arg[3]);
}

void
ioopm_add_cleaners(ioopm_list_t *list, ioopm_transform_value i_clean_value, 
                   ioopm_transform_value i_clean_key)
{
    list->clean_key = i_clean_key;
    list->clean_value = i_clean_value;
}

/**
 * @brief Function to actually test whether to filter out each node
 * 
 * @param remove_node 
 * @param success 
 * @param arg 
 */
static
void 
filter(node_t *remove_node, bool *success, void **arg)
{
    //This does not track index...
    ioopm_pred_value pred = arg[0];
    ioopm_list_t *list = arg[1];
    //idea is that it should not eval next statement... NULL IS A FREE PASS TO CHANGE ALL
    if(!pred || pred(remove_node->value, arg[2]))
    {    
        //contract to isolate remove node
        contract(remove_node);

        //clean the data of the node so that the heap can be reused
        clean_data(&(remove_node->value), &(remove_node->key), 
                   list->clean_value, list->clean_key);

       list->size--;
    }
}

/**
 * @brief This will be our factory pipeline, we put each element in the list into the pipe
 * and make some change to it.
 * 
 * @param list
 * @param success Communicate outcome of pipelining all relevant elements to operator function
 * @param func - This is the general handler that applies a specific function in arg.
 * @param arg Array of void pointers minimum size is two, the first slot is for function 
 * used by handler.
 * @return true 
 * @return false 
 */
static
void 
pipeline(ioopm_list_t *list, bool *success, handler func, void **arg)
{
    if(list->size <= 0) return;

    node_t *node_to_handle = list->first->next;

    while(node_to_handle->next)
    {
        node_t *temp = node_to_handle->next;
        func(node_to_handle, success, arg);
        node_to_handle = temp;
    }
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_pred_value prop, void *extra)
{
    bool success = true;
    void *arg[] = {prop, extra};
    pipeline(list, &success, true_all, arg);
    return success;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_pred_value prop, void *extra)
{
    bool success = false;
    void *arg[] = {prop, extra};
    pipeline(list, &success, true_any, arg);
    return success;
}



bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element, ioopm_pred_value comparer)
{
    return ioopm_linked_list_any(list, comparer, &element);
}


void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_transform_value fun, 
                                    void *extra)
{
    if(!list || !fun) return;

    ioopm_list_apply_extended(list, fun, extra, NULL, NULL);
}

void 
ioopm_list_apply_extended(ioopm_list_t *list, 
                               ioopm_transform_value fun_value, void *extra_value,
                               ioopm_transform_value fun_key, void *extra_key)
{
    bool placeholder;
    void *arg[] = {fun_value, fun_key, extra_value, extra_key};
    pipeline(list, &placeholder, transform_list_node, arg);
}


void 
ioopm_linked_list_clear(ioopm_list_t *list)
{
    if(!list)
        return;
    ioopm_filter_all(list, NULL, NULL);
}

void 
ioopm_filter_all(ioopm_list_t *list, ioopm_pred_value pred, void *extra)
{
    bool success;
    void *arg[] = {pred, list, extra};
    pipeline(list, &success, filter, arg);
}


void
ioopm_append_lists(ioopm_list_t *listA, ioopm_list_t *listB, bool take_key)
{
    bool choice = take_key;
    void *arg[] = {listA};
    pipeline(listB, &choice, append_node, arg);    
}

elem_t
ioopm_element_to_list(elem_t list, elem_t item)
{

    ioopm_list_t *list_to_add = list.p;
    ioopm_linked_list_append(list_to_add, item, (elem_t) NULL);
    return list;
}


