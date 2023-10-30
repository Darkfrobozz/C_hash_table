#include "include/iterator.h"
#include "include/nodes.h"
#include <stdlib.h>

enum itertypes{list_iter, array_iter};

struct iterator
{
    enum itertypes type;
    void *datastructure;
    void *current_adress;
    bool dummied;
};

/**
 * @brief Returns whether initiation is successful or not needs to be called
 * because we allow creation of cursor over empty lists.
 * Moves to first element.
 * @param iter 
 * @return true rest of program should execute
 * @return false no execute
 */
static
bool 
iterator_init(ioopm_iterator_t *iter)
{
    assert(iter);
    if(!(iter->dummied))
        return true;
    ioopm_list_t *list = iter->datastructure;
    //check if empty
    if(ioopm_linked_list_is_empty(list))
        return false;
    //check 
    node_t *node = iter->current_adress;

    iter->current_adress = node->next;
    iter->dummied = false;
    return true;
}
static
void
ioopm_iterator_list_clear(elem_t *value, void *arg)
{
    ioopm_iterator_t *iter = value->p;
    free(iter);
    *value = (elem_t) NULL;
}

ioopm_iterator_t *
ioopm_list_iterator(ioopm_list_t *list)
{

    ioopm_iterator_t *iter = calloc(1, sizeof(ioopm_iterator_t));
    iter->current_adress = list->first;
    void *added_iter = iter;
    if(!(list->iterator_list))
    {
        list->iterator_list = ioopm_linked_list_create();
        ioopm_add_cleaners(list->iterator_list, ioopm_iterator_list_clear, NULL);
    }
    ioopm_linked_list_append(list->iterator_list, (elem_t) added_iter, (elem_t) NULL);
    iter->datastructure = list;
    iter->type = list_iter;
    iter->dummied = true;
    iterator_init(iter); 
    return iter;
}

void 
ioopm_iterator_destroy(ioopm_iterator_t *iter)
{
    if(iter->type == list_iter)
    {
        ioopm_list_t *list = iter->datastructure;
        ioopm_filter_all(list->iterator_list, ioopm_equals_adress, iter);
    }
}

void 
ioopm_inform_removal(elem_t *value, void *removed_node)
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
            if(!(current_adress->previous->previous))
                iter->dummied = true;
        }
    }
}
static
bool 
ioopm_iterator_has(ioopm_iterator_t *iter, short dir_command)
{
    if(!iterator_init(iter)) 
        return false;
    node_t *node = iter->current_adress;

    if(dir_command == LEFT)
    {
        if(node->previous->previous)
            return true;

        return false;
    } 

    if(node->next->next)
        return true;
    
    return false;

}

bool 
ioopm_iterator_has_next(ioopm_iterator_t *iter)
{
    return ioopm_iterator_has(iter, RIGHT);
}

bool 
ioopm_iterator_has_prev(ioopm_iterator_t *iter)
{
    return ioopm_iterator_has(iter, LEFT);
}

option_t 
ioopm_iterator_next(ioopm_iterator_t *iter)
{
    option_t result = {0};

    if(!iterator_init(iter)) 
        return result; //making sure that if the list ain't empty we will be on a valid spot.
    if(!ioopm_iterator_has_next(iter))
        return result;
    
    node_t *node = iter->current_adress;
    iter->current_adress = node->next;    

    return ioopm_iterator_current_value(iter); //if next is dummy node
}

option_t 
ioopm_iterator_previous(ioopm_iterator_t *iter)
{
    option_t result = {0};

    if(!iterator_init(iter)) 
        return result; 
    //making sure that if the list ain't empty we will be on a valid spot.
    if(!ioopm_iterator_has_prev(iter))
        return result;
    
    node_t *node = iter->current_adress;
    iter->current_adress = node->previous;
    
    return ioopm_iterator_current_value(iter);
}

void
ioopm_iterator_edit(ioopm_iterator_t *iter, ioopm_transform_value transformation, 
                    void *arg)
{
    ioopm_list_t *list = iter->datastructure;
    //we need to free data we replace..
    ioopm_edit_node_value(list, 
                          transformation, 
                          (node_t *) iter->current_adress, arg);
}

option_t 
ioopm_iterator_insert(ioopm_iterator_t *iter, elem_t value, elem_t key, short dir)
{
    option_t result = {0};
    bool ans = iterator_init(iter);
    node_t *to_send = iter->current_adress;
    
    //if list is empty just insert
    if((dir == LEFT) && ans)
        to_send = to_send->previous;

    result = ioopm_insert_node(to_send, value, 
                      key, (ioopm_list_t  *) iter->datastructure);
    
    iter->current_adress = to_send->next;
    
    return result;
}


option_t 
ioopm_iterator_remove(ioopm_iterator_t *iter)
{
    if(!iterator_init(iter)) 
        return (option_t) {0};
    return ioopm_remove_node((ioopm_list_t *) iter->datastructure, 
                             (node_t *) iter->current_adress);
}

void 
ioopm_iterator_reset(ioopm_iterator_t *iter)
{
    ioopm_list_t *list = iter->datastructure;

    if(!ioopm_linked_list_is_empty(list))
    iter->current_adress = list->first;
    iter->dummied = true;
    iterator_init(iter);
}

option_t 
ioopm_iterator_current_value(ioopm_iterator_t *iter)
{
    if(!iterator_init(iter)) 
        return (option_t) {0};
    node_t *node = iter->current_adress;
    return (option_t) {.success = 1, .return_value = node->value};
}

option_t 
ioopm_iterator_current_key(ioopm_iterator_t *iter)
{
    if(!iterator_init(iter)) 
        return (option_t) {0};
    node_t *node = iter->current_adress;
    return (option_t) {.success = 1, .return_value = node->key};
}
void
ioopm_iter_apply_destroy(elem_t *value, void *arg)
{
    ioopm_iterator_t *iter = value->p;
    ioopm_iterator_destroy(iter);
}



void
ioopm_iter_destroy_list(ioopm_iterator_t *iter)
{
    ioopm_list_t *list = iter->datastructure;
    ioopm_linked_list_clear(list);
    ioopm_linked_list_destroy(list);
}

size_t
ioopm_iter_list_siz(ioopm_iterator_t *iter)
{
    ioopm_list_t *list = iter->datastructure;
    return ioopm_linked_list_size(list);
}