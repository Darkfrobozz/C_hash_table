#include "include/iterator.h"
#include "include/nodes.h"
#include <stdlib.h>
#define START 1
#define LAST -1

enum itertypes{list_iter, array_iter};

struct iterator
{
    enum itertypes type;
    //Adresses to relevant structures
    void *d_struct;
    void *c_adress;
    //What index, start is 0
    int index;
    //If iterator is on a dummy node
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
iterator_list_init(ioopm_iterator_t *iter)
{
    assert(iter);
    if(!(iter->dummied))
        return true;
    ioopm_list_t *list = iter->d_struct;
    //check if empty
    if(ioopm_linked_list_is_empty(list))
        return false;
    //check 
    node_t *node = iter->c_adress;

    iter->c_adress = node->next;
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

static
void
ioopm_iterator(ioopm_iterator_t *iter, ioopm_list_t **iterator_list, 
               void *first_element, void *data)
{
    iter->d_struct = data;
    iter->c_adress = first_element;
    if(!(*iterator_list))
    {        
        *iterator_list = ioopm_linked_list_create();
        ioopm_add_cleaners(*iterator_list, ioopm_iterator_list_clear, NULL);
    }
    ioopm_linked_list_append(*iterator_list, 
                            (elem_t) (void *) iter, (elem_t) NULL);

}


ioopm_iterator_t *
ioopm_list_iterator(ioopm_list_t *list)
{
    ioopm_iterator_t *iter = calloc(1, sizeof(ioopm_iterator_t));
    iter->type = list_iter;
    ioopm_iterator(iter, &(list->iterator_list), list->first, list);

    iter->dummied = true;
    return iter;
}

ioopm_iterator_t *
ioopm_array_iterator(array_t *array)
{
    ioopm_iterator_t *iter = calloc(1, sizeof(ioopm_iterator_t));
    iter->type = array_iter;
    ioopm_iterator(iter, &(array->iterator_list), 
                   array->p_first_element, array);

    return iter;
}

void 
ioopm_iterator_destroy(ioopm_iterator_t *iter)
{
    switch(iter->type){
        case list_iter:
        {
            ioopm_list_t *list = iter->d_struct;
            ioopm_filter_all(list->iterator_list, ioopm_equals_adress, iter);
            break;
        }

        case array_iter:
        {
            array_t *arr = iter->d_struct;
            ioopm_filter_all(arr->iterator_list, ioopm_equals_adress, iter);
            break;
        }
    } 
}

//This is used in list
//We can create an iterator for pipe
void 
ioopm_inform_removal(elem_t *value, void *removed_node)
{
    
    ioopm_iterator_t *iter = value->p;

    elem_t adress_removed;
    adress_removed.p = removed_node;

    if(ioopm_equals_adress(adress_removed, iter->c_adress))
    {
        if(iter->type == list_iter)
        {
            node_t *c_adress = iter->c_adress;
            iter->c_adress = c_adress->previous;
            if(!(c_adress->previous->previous))
                iter->dummied = true;
        }
    }
}

/**
 * @brief Controls iter direction possibilities
 * 
 * @param iter 
 * @param dir 
 * @return true 
 * @return false 
 */
static
bool 
ioopm_iterator_has(ioopm_iterator_t *iter, short dir)
{

    switch (iter->type) {
        case list_iter:
        {
            node_t *node = iter->c_adress;
            if(!iterator_list_init(iter)) 
                return false;        
            if(dir == LEFT && node->previous->previous)
                return true;
            else if(dir == RIGHT && node->next->next)
                return true;
            return false;
        }

        case array_iter:
        {
            array_t *array = iter->d_struct;
            if(dir == LEFT && (iter->c_adress != array->p_first_element))
                return true;
            else if(dir == RIGHT && iter->c_adress != array->p_last_element)
                return true;
            return false;
        }
        default:
        assert(false);
    }

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

/**
 * @brief Moves c_adress based on type and linear direction
 * 
 * @param type 
 * @param c_adress 
 * @param data 
 * @param dir 
 */
static
void
ioopm_move_c_adress(ioopm_iterator_t *iter, short dir)
{
    void *data = iter->d_struct; 
    void *c_adress = iter->c_adress;
    switch(iter->type){
        case list_iter:
        if(dir == RIGHT)
        {
            iter->c_adress = ((node_t *) c_adress)->next; 
            iter->index++;
        }
        else
        {
            iter->c_adress = ((node_t *) c_adress)->previous; 
            iter->index--;
        }
        
        break;

        case array_iter:
            iter->c_adress = (char *) c_adress 
                             + dir * (((array_t *) data)->chunk_siz);
            iter->index += dir;
                            
        break;
        default:
        assert(false);
    }
}

option_t 
ioopm_iterator_next(ioopm_iterator_t *iter)
{
    option_t result = {0};

    //Checking if have next
    if(!ioopm_iterator_has_next(iter))
        return result;
    
    ioopm_move_c_adress(iter, RIGHT);

    return ioopm_iterator_current_value(iter); //if next is dummy node
}

option_t 
ioopm_iterator_previous(ioopm_iterator_t *iter)
{
    option_t result = {0};

    //Checking if has prev
    if(!ioopm_iterator_has_prev(iter))
        return result;
    
    ioopm_move_c_adress(iter, LEFT); 
    
    return ioopm_iterator_current_value(iter);
}

void
ioopm_iterator_edit(ioopm_iterator_t *iter, 
                    ioopm_transform_value transformation, 
                    void *arg)
{
    switch(iter->type){
        case list_iter:
        {
            ioopm_list_t *list = iter->d_struct;
            //we need to free data we replace..
            //how do we edit key
            ioopm_edit_node_value(list, 
                                    transformation, 
                                    (node_t *) iter->c_adress, arg);
            break;
        }

        case array_iter:
        {
            array_t *arr = iter->d_struct;
            if(!transformation)
                memcpy(iter->c_adress, arg, arr->chunk_siz);
            else
            {
                elem_t elem_edit = arr->cast(iter->c_adress);
                transformation(&elem_edit, arg);
                memcpy(iter->c_adress, &elem_edit, arr->chunk_siz);
            }
        }
    }

}

option_t 
ioopm_iterator_remove(ioopm_iterator_t *iter)
{
    switch (iter->type) {
        case list_iter:
            if(!iterator_list_init(iter)) 
                return (option_t) {0};

            return ioopm_remove_node((ioopm_list_t *) iter->d_struct, 
                                    (node_t *) iter->c_adress);

        case array_iter:
        {
            array_t *arr = iter->d_struct;
            memset(iter->c_adress, 0, arr->chunk_siz);
            return (option_t) {.success = 1};
        }
        default:
        assert(false);
    }
}
static
void
reset(ioopm_iterator_t *iter, short to_cm)
{
    if(to_cm == START)
        ioopm_iterator_set(iter, 0);
    if(to_cm == LAST)
        ioopm_iterator_set(iter, ioopm_iter_db_siz(iter) - 1);
}

void 
ioopm_iterator_reset(ioopm_iterator_t *iter)
{
    reset(iter, START);
}

void
ioopm_iterator_last(ioopm_iterator_t *iter)
{
    reset(iter, LAST);
}

option_t 
ioopm_iterator_current_value(ioopm_iterator_t *iter)
{
    switch (iter->type) {
        case list_iter:
        {
            if(!iterator_list_init(iter)) 
                return (option_t) {0};
            node_t *node = iter->c_adress;
            return (option_t) {.success = 1, .return_value = node->value};
        }
        case array_iter:
        {
            array_t *arr = iter->d_struct;
            return (option_t) {.success = 1,
                               .return_value = arr->cast(iter->c_adress)};

        }

        default:
        assert(false);
    }
}

size_t
ioopm_iter_db_siz(ioopm_iterator_t *iter)
{
    switch (iter->type) {
        case list_iter:
        {
            ioopm_list_t *list = iter->d_struct;
            return ioopm_linked_list_size(list);
        }
        
        case array_iter:
        {
            array_t *arr = iter->d_struct;
            return arr->elements;
        } 

        default:
        assert(false);
    }
}
void
ioopm_iter_destroy_db(ioopm_iterator_t *iter)
{   
    switch (iter->type) {
        case list_iter:
        {
            ioopm_list_t *list = iter->d_struct;
            ioopm_linked_list_clear(list);
            ioopm_linked_list_destroy(list);
            break;
        }
        case array_iter:
        {
            array_t *arr = iter->d_struct;
            ioopm_array_destroy(arr);
            break;
        }
        default:
        assert(false);
    }
}

//ONLY 0 and list size is supported for list, iterator has every index
bool
ioopm_iterator_set(ioopm_iterator_t *iter, int index)
{
    //perhaps have this in array
    switch (iter->type) {
        case (list_iter):
        {
            ioopm_list_t *list = iter->d_struct;
            int siz = ioopm_linked_list_size(list);
            if(siz == 0)
                return false;
            if(index == 0)
            {
                iter->c_adress = list->first->next;
                iter->dummied = false;
                return true;
            }
            if(index == siz - 1)
            {
                iter->c_adress = list->last->previous;
                iter->dummied = false;
                return true;
            }
            return false;
        }
        case (array_iter):
        {    
            array_t *arr = iter->d_struct;
            if(index < 0 || index >= arr->elements)
                return false;
            iter->c_adress = ((char *) arr->p_first_element) + index * arr->chunk_siz;
            iter->index = index;
            return true;
        }
        default:
        assert(false);
    }
}

//ONLY LIST
option_t
static
iterator_list_add(ioopm_iterator_t *iter, elem_t value, elem_t key, short dir)
{
    option_t result = {0};
    bool ans; 
    node_t *to_send; 
    if(iter->type == array_iter)
        return result;

    ans = iterator_list_init(iter);
    to_send = iter->c_adress;
    
    //if list is empty just insert
    if((dir == LEFT) && ans)
        to_send = to_send->previous;

    result = ioopm_insert_node(to_send, value, 
                      key, (ioopm_list_t  *) iter->d_struct);
    
    iter->c_adress = to_send->next;
    
    return result;
}

option_t 
ioopm_iterator_insert(ioopm_iterator_t *iter, elem_t value, elem_t key)
{
    return iterator_list_add(iter, value, key, RIGHT);
}

option_t 
ioopm_iterator_prepend(ioopm_iterator_t *iter, elem_t value, elem_t key)
{
    return iterator_list_add(iter, value, key, LEFT);
}

option_t 
ioopm_iterator_current_key(ioopm_iterator_t *iter)
{
    option_t result = {0};
    switch(iter->type){
        case list_iter:
        {
            if(!iterator_list_init(iter)) 
                return result;
            node_t *node = iter->c_adress;
            return (option_t) {.success = 1, .return_value = node->key};
        }
        case array_iter:
            return result;
        default:
        assert(false);
    } 
}

void
ioopm_iter_apply_destroy(elem_t *value, void *arg)
{

    ioopm_iterator_t *iter = value->p;
    ioopm_iterator_destroy(iter);
}