#include "linked_list.h"
#include "iterator.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef void(*handler)(node_t *node, bool *success, void **arg);


ioopm_list_t *ioopm_linked_list_create(void)
{
    ioopm_list_t *result = calloc(1, sizeof(ioopm_list_t));
    node_t *first = calloc(1, sizeof(node_t));
    node_t *last = calloc(1, sizeof(node_t));
    result->first = first;
    first->next = last;
    last->previous = first;
    result->last = last;
    return result;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    if(!list) return;
    ioopm_linked_list_destroy(list->iterator_list);
    free(list->first);
    free(list->last);
    free(list);
}

/**
 * @brief Responsible for traversing the list either from last node or first node 
 * 
 * @param starting_node - The last or first dummy node.
 * @param start_index - The start or the end.
 * @param dir Decides whether we are moving left or right in a linked list
 * @param goal_index Minus one of the index given by the user 
 * since we want to find the previous node
 * 
 * @return node_t* A pointer to the previous node
 */
node_t *traverse_list(node_t *starting_node, int start_index, int dir, int goal_index)
{
    while(start_index != goal_index)
    {
        if(dir == -1)
        {
            starting_node = starting_node->previous;
        }
    
        if(dir == 1)
        {
            starting_node = starting_node->next;
        }

        start_index = start_index + dir;
    }
    return starting_node;

}


/// @brief Find the previous node of a node at a specific index and tells traverse list how to do its job
/// @param list
/// @param index
node_t *find_previous_node(ioopm_list_t *list, int index)
{

    if(index <= (list->size / 2))
    {
        return traverse_list(list->first, -1, 1, index - 1);
    }

    return traverse_list(list->last, list->size, -1, index - 1);

}


option_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    option_t result = {0};
    if(list == NULL || index < 0 || index >= list->size)
    {
        return result;
    }

    node_t *prev_node = find_previous_node(list, index);

    result.return_value = prev_node->next->value;
    result.success = 1;

    return result;

}
option_t 
ioopm_insert_node(node_t *prev_node, elem_t i_value, elem_t i_key, ioopm_list_t *list)
{
    option_t result = {0};
    node_t *insert = calloc(1, sizeof(node_t));
    insert->value = i_value;
    insert->key = i_key;



    node_t *next_node = prev_node->next;

    prev_node->next = insert;
    next_node->previous = insert;
    insert->next = next_node;
    insert->previous = prev_node;

    list->size++;

    result.success = 1;
    result.return_value = i_value;

    return result;
    
}

option_t 
ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t i_value, elem_t i_key)
{
    option_t result = {.return_value = i_value, .success = 0};
    if(index > list->size || index < 0)
    {

        return result;
    }

    node_t *prev_node = find_previous_node(list, index);
    
    return ioopm_insert_node(prev_node, i_value, i_key, list);
}


option_t 
ioopm_linked_list_append(ioopm_list_t *list, elem_t i_value, elem_t i_key)
{
    int index = list->size;
    
    return ioopm_linked_list_insert(list, index, i_value, i_key);
    
}


option_t ioopm_linked_list_prepend(ioopm_list_t *list, elem_t i_value, elem_t i_key)
{
    return ioopm_linked_list_insert(list, 0, i_value, i_key);
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return (list->size == 0);
}

option_t ioopm_remove_node(ioopm_list_t *list, node_t *remove_node)
{
    option_t result = {0};

    node_t *prev_node = remove_node->previous;
    node_t *next_node = remove_node->next;

    result.return_value = remove_node->value;

    //informing iterators
    ioopm_linked_list_apply_to_all(list->iterator_list, ioopm_inform_removal, remove_node);

    free(remove_node);
    list->size--;

    prev_node->next = next_node;
    next_node->previous = prev_node;

    result.success = 1;

    return result;
   
}

option_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{

    option_t result = {0};
    if(index >= list->size || index < 0 || list->size == 0)
    {
        return result;
    }

    node_t *remove_node = find_previous_node(list, index)->next;
    
    result = ioopm_remove_node(list, remove_node);

    return result;
}

ioopm_list_t *ioopm_get_iterator(ioopm_list_t *list)
{
    return list->iterator_list;
}
/**
 * @brief Handler function to apply a predicate function
 * 
 * @param node To apply to
 * @param success Communicate outcome of predicate
 * @param arg Contains predicate to apply and extra arguments to it
 */
void true_all(node_t *node, bool *success, void **arg)
{
    ioopm_predicate_list pred = arg[0];
    if(!pred(node->value, arg[1]))
    {
        *success = false;
    }
}

//similar to true all
void true_any(node_t *node, bool *success, void **arg)
{
    ioopm_predicate_list pred = arg[0];
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

void transform_extended(node_t *node, bool *success, void **arg)
{
    ioopm_apply_function_list func_value = arg[0];
    ioopm_apply_function_list func_key = arg[1];
    if(func_value)
        func_value(&(node->value), arg[2]);
    if(func_key)
        func_key(&(node->key), arg[3]);
}


void clear_node(node_t *node, bool *success, void **arg)
{
    free(node);
}

void destroy_iter(node_t *node, bool *success, void **arg)
{
    ioopm_iterator_t *iter = node->value.p;
    ioopm_iterator_destroy(iter);
}


void filter(node_t *node, bool *success, void **arg)
{
    ioopm_predicate_list pred = arg[0];
    ioopm_list_t *list = arg[1];
    if(pred(node->value, arg[2]))
    {
        node_t *next_node = node->next;
        node_t *prev_node = node->previous;
        next_node->previous = prev_node;
        prev_node->next = next_node;
        clear_node(node, success, arg);
        (list->size)--;
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
void pipeline(ioopm_list_t *list, bool *success, handler func, void **arg)
{
    if(list->size <= 0) return;

    node_t *node_to_handle = list->first->next;

    while(node_to_handle != list->last)
    {
        node_t *temp = node_to_handle->next;
        func(node_to_handle, success, arg);
        node_to_handle = temp;
    }
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate_list prop, void *extra)
{
    bool success = true;
    void *arg[] = {prop, extra};
    pipeline(list, &success, true_all, arg);
    return success;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate_list prop, void *extra)
{
    bool success = false;
    void *arg[] = {prop, extra};
    pipeline(list, &success, true_any, arg);
    return success;
}



bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element, ioopm_predicate_list comparer)
{
    return ioopm_linked_list_any(list, comparer, &element);
}


void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function_list fun, 
                                    void *extra)
{
    if(!list || !fun) return;

    ioopm_list_apply_extended(list, fun, extra, NULL, NULL);
}

void ioopm_list_apply_extended(ioopm_list_t *list, 
                               ioopm_apply_function_list fun_value, void *extra_value,
                               ioopm_apply_function_list fun_key, void *extra_key)
{
    bool placeholder;
    void *arg[] = {fun_value, fun_key, extra_value, extra_key};
    pipeline(list, &placeholder, transform_extended, arg);
}


void ioopm_linked_list_clear(ioopm_list_t *list)
{
    if(!list) return;
    bool success = true;
    pipeline(list, &success, clear_node, NULL);
    list->first->next = list->last;
    list->last->previous = list->first;
    list->size = 0;
}

void ioopm_filter_all(ioopm_list_t *list, ioopm_predicate_list pred, void *extra)
{
    bool success;
    void *arg[] = {pred, list, extra};
    pipeline(list, &success, filter, arg);
}

void
ioopm_list_clear_and_destroy(ioopm_list_t *list, ioopm_predicate_list pred, void **arg)
{
    if(list)
    {
        bool placeholder;
        ioopm_list_t *iterator_list = ioopm_get_iterator(list);

        if(iterator_list)
        {
            pipeline(iterator_list, &placeholder, destroy_iter, NULL);
            ioopm_linked_list_clear(iterator_list);
        }

        ioopm_linked_list_clear(list);
        ioopm_linked_list_destroy(list); //Destroys all lists recursively
    }
}


void
ioopm_append_lists(ioopm_list_t *listA, ioopm_list_t *listB, bool take_key)
{
    bool choice = take_key;
    void *arg[] = {listA};
    pipeline(listB, &choice, append_node, arg);
    
}

