#include "iterator.h"
#include "linked_list.h"
#include <stdlib.h>

enum itertypes{list_iter, hash_iter};

struct iterator
{
    enum itertypes type;
    void *datastructure;
    void *current_adress;
};
/**
 * @brief Returns whether initiation is successful or not needs to be called
 * because we allow creation of cursor over empty lists.
 * Moves to first element.
 * @param iter 
 * @return true rest of program should execute
 * @return false no execute
 */
bool iterator_init(ioopm_iterator_t *iter)
{
    node_t *node = iter->current_adress;
    if(node->previous) return true; //not in start
    if(node->next->next) //not at end at start
    {
        iter->current_adress = node->next;
        return true;
    }
    return false; // in start and at end cannot init
}

ioopm_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{

    ioopm_iterator_t *iter = calloc(1, sizeof(ioopm_iterator_t));
    iter->current_adress = list->first;
    void *added_iter = iter;
    if(!(list->iterator_list))
    {
        list->iterator_list = ioopm_linked_list_create();
    }
    ioopm_linked_list_append(list->iterator_list, (elem_t) added_iter);
    iter->datastructure = list;
    iter->type = list_iter;
    iterator_init(iter);
    return iter;
}

void ioopm_iterator_destroy(ioopm_iterator_t *iter)
{
    if(iter->type == list_iter)
    {
        ioopm_list_t *list = iter->datastructure;
        ioopm_filter_all(list->iterator_list, ioopm_equals_adress, iter);
    }

    free(iter);
}

void ioopm_inform_removal(elem_t *value, void *removed_node)
{
    
    ioopm_iterator_t *iter = value->p;

    elem_t adress_removed;
    adress_removed.p = removed_node;

    if(ioopm_equals_adress(adress_removed, iter->current_adress))
    {
        if(iter->type == list_iter)
        {
            node_t *current_adress = iter->current_adress;
            iter->current_adress = current_adress->previous;
        }
    }
}

bool ioopm_iterator_has_next(ioopm_iterator_t *iter)
{
    if(!iterator_init(iter)) return false;
    node_t *node = iter->current_adress;
    node_t *next_node = node->next;

    if(next_node->next) // check if next is a dummy_node
    {
        return true;
    }
    return false;
}

option_t ioopm_iterator_next(ioopm_iterator_t *iter)
{
    option_t result = {0};
    if(!iterator_init(iter)) return result; //making sure that if the list ain't empty we will be on a valid spot.

    node_t *node = iter->current_adress;
    node_t *next_node = node->next;

    if(next_node->next) //check if next is not dummy node
    {
        iter->current_adress = node->next; //then move forward

        node = iter->current_adress; //new node
        result.success = 1;
        result.return_value = node->value;
        return result;
    }

    return result; //if next is dummy node
}

option_t ioopm_iterator_previous(ioopm_iterator_t *iter)
{
    option_t result = {0};
    if(!iter) return result; 
    //making sure that if the list ain't empty we will be on a valid spot.
    node_t *node = iter->current_adress;
    node_t *prev_node = node->previous;
    //Checking if we on first dummynode
    if(prev_node)
    {
        iter->current_adress = prev_node;
        result.return_value = prev_node->value;
        result.success = 1;
    }

    return result;
}

option_t ioopm_iterator_insert(ioopm_iterator_t *iter, elem_t value, elem_t key)
{
    option_t result = {0};
    if(!iter) return result;
    result = ioopm_insert_node((node_t *) iter->current_adress, 
    value, (ioopm_list_t *) iter->datastructure);
    node_t *prev = iter->current_adress;
    node_t *inserted = prev->next;
    inserted->key = key;
    return result;
}

option_t ioopm_iterator_remove(ioopm_iterator_t *iter)
{
    option_t result = {0};
    if(!iterator_init(iter)) return result;
    if(!iter) return result;
    return ioopm_remove_node((ioopm_list_t *) iter->datastructure, (node_t *) iter->current_adress);
}

void ioopm_iterator_reset(ioopm_iterator_t *iter)
{
    iter->current_adress = ioopm_list_first((ioopm_list_t *) iter->datastructure);
}

option_t ioopm_iterator_current(ioopm_iterator_t *iter)
{
    option_t result = {0};
    if(!iterator_init(iter)) return result;
    node_t *node = iter->current_adress;
    result.return_value = node->value;
    result.success = 1;
    return result; // You have come to the end of the list
}

option_t ioopm_iterator_current_key(ioopm_iterator_t *iter)
{
    option_t result = {0};
    if(!iterator_init(iter)) return result;
    node_t *node = iter->current_adress;
    result.return_value = ioopm_get_key(node);
    result.success = 1;
    return result; // You have come to the end of the list
}
