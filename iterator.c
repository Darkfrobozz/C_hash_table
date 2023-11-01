#include "include/iterator.h"
#include "include/automaton.h"
#include "include/nodes.h"
#include <stdlib.h>
#define START 1
#define STAY 0
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

static
void
iter_list_dummy(ioopm_iterator_t *iter)
{
    ioopm_list_t *list = iter->d_struct;
    iter->c_adress = list->first;
    iter->dummied = true;
}

/**
 * @brief Used to init iterator
 * 
 * @param iter 
 * @param command 
 * 1 -> Stay
 * 2 -> Set to list first next
 * 3 -> set to list last previous
 */
static
bool
iterator_list_set(ioopm_iterator_t *iter, short command)
{
ioopm_list_t *list = iter->d_struct;

switch(command){
    case STAY: 
    return true;

    case START:
    if(!ioopm_iter_db_siz(iter))
    {
        iter_list_dummy(iter);
        return false;
    }
    iter->index = 0;
    iter->c_adress = list->first->next;
    iter->dummied = false;
    return true;

    case LAST:
    {
        int temp = ioopm_iter_db_siz(iter);
        if(!ioopm_iter_db_siz(iter))
        {
            iter_list_dummy(iter); 
            return false;
        }
        iter->index = temp - 1;
        iter->c_adress = list->last->previous;
        iter->dummied = false;
        return true;
    }        

    default:
    assert(false);
}
}


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
    //check if empty
    if(!(ioopm_iter_db_siz(iter)))
        return false;
    
    //Set to start if it was dummied 
    iterator_list_set(iter, START);

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
ioopm_iterator_t *
iterator(ioopm_iterator_t *iter, ioopm_list_t **iterator_list, 
               void *data)
{
    iter->d_struct = data;
    ioopm_iterator_reset(iter);
    if(!(*iterator_list))
    {        
        *iterator_list = ioopm_linked_list_create();
        ioopm_add_cleaners(*iterator_list, ioopm_iterator_list_clear, NULL);
    }
    ioopm_linked_list_append(*iterator_list, 
                            (elem_t) (void *) iter, (elem_t) NULL);
    return iter;
}


ioopm_iterator_t *
ioopm_list_iterator(ioopm_list_t *list)
{
    ioopm_iterator_t *iter = calloc(1, sizeof(ioopm_iterator_t));
    iter->type = list_iter;
    return iterator(iter, &(list->iterator_list), list);
}

ioopm_iterator_t *
ioopm_array_iterator(array_t *array)
{
    ioopm_iterator_t *iter = calloc(1, sizeof(ioopm_iterator_t));
    iter->type = array_iter;
    iter->c_adress = array->p_first_element;
    return iterator(iter, &(array->iterator_list), array);
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
    //Indexes will be shifted, iters ahead of current iterator
    //will be wrong! Inform removal should carry both removed_node and index
    //Send index instead for removal!!! Then we can just compare index
    ioopm_iterator_t *iter = value->p;
    option_t result; 


    if(!(iter->c_adress == removed_node))
        return;
    
    assert(ioopm_iter_db_siz(iter)); 

    result = ioopm_iterator_previous(iter);

    if(!result.success)
        result = ioopm_iterator_next(iter);
    if(!result.success)
        iter_list_dummy(iter);
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

option_t
ioopm_iterator_edit(ioopm_iterator_t *iter, 
                    ioopm_transform_value transformation, 
                    void *arg)
{
    if(!ioopm_iter_db_siz(iter))
        return (option_t) {0};
    switch(iter->type){
        case list_iter:
        {
            ioopm_list_t *list = iter->d_struct;
            //we need to free data we replace..
            //how do we edit key
            return ioopm_edit_node_value(list, 
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
            return (option_t) {.return_value = (elem_t) iter->c_adress, .success = 1};
        }

        default:
        assert(false);
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
            return ioopm_iterator_next(iter);
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

int
ioopm_iter_index(ioopm_iterator_t *iter)
{
    return iter->index;
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

/**
 * @brief Find smallest of 
 * 
 * @param c_to_g returns STAY 
 * @param s_to_g returns START
 * @param e_to_g returns LAST
 * @return short 
 */
short
static
three_way_comparator(int c_to_g, int s_to_g, int e_to_g)
{
    if(c_to_g < e_to_g)
    {
        if(c_to_g < s_to_g)
            return STAY;
        return START;
    }
    if(s_to_g < e_to_g)
        return START;
    return LAST;
}


bool
ioopm_iterator_set(ioopm_iterator_t *iter, int index)
{
    //perhaps have this in array
    switch (iter->type) {
        case (list_iter):
        {
            int temp = ioopm_iter_db_siz(iter);
            int s_to_g = abs(index);
            int e_to_g; 
            int c_to_g;

            if(!s_to_g)
                return iterator_list_set(iter, START);
            e_to_g = abs(temp - 1 - index);
            if(!e_to_g)
                return iterator_list_set(iter, LAST);
            
            c_to_g = abs(iter->index - index);

            if(!c_to_g)
                return true;
            
            temp = three_way_comparator(c_to_g, s_to_g, e_to_g);
 
            return iterator_list_set(iter, temp);

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
    iter->dummied = false;
    
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

//ADVANCED FUNCTIONS
void
ioopm_move_iter_index(ioopm_iterator_t *iter, int index)
{  
    ioopm_list_t *assembly_list = ioopm_linked_list_create();
    ioopm_linked_list_append(assembly_list, (elem_t) index, 
                             (elem_t) (void *) ioopm_fast_index);
    ioopm_run_automaton(iter, assembly_list);
    ioopm_linked_list_clear(assembly_list);
    ioopm_linked_list_destroy(assembly_list);
}