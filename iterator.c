#include "include/automaton.h"
#include "include/nodes.h"
#include <stdlib.h>
//MACROS
#define node(x) ((node_t *)(x->c_adress))
#define list(x) ((ioopm_list_t *)(x->d_struct))
#define array(x) ((array_t *)(x->d_struct))

#define set_last(x) ((x->c_adress) = (list(x)->last->previous))
#define set_first(x) ((x->c_adress) = (list(x)->last->next))
#define at(x) (x->c_adress)
#define i_next(x, a) (x->index + a)
#define siz(x) (ioopm_iter_db_siz(x))
#define chunk(x) (array(x)->chunk_siz)
#define within(a,x,b) (((x) >= (a)) && ((x) <= (b)))
#define end_p(x) (siz(x) - 1)
#define direction(to, x) (to - i_next(x, 0))


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
    ioopm_list_t *iterator_list;
};

static
void
tracking_list(ioopm_iterator_t *iter, ioopm_list_t *list)
{
    iter->type = list_iter;
    iter->d_struct = list;
    iter->dummied = true;
    iter->iterator_list = list_track_iter(list, iter);
}

static
void
tracking_array(ioopm_iterator_t *iter, array_t *array)
{
    iter->type = array_iter;
    iter->c_adress = array->p_first_element;
    iter->d_struct = array;
    iter->iterator_list = array_track_iter(array, iter);
}


ioopm_iterator_t *
ioopm_list_iterator(ioopm_list_t *list)
{
    ioopm_iterator_t *iter = calloc(1, sizeof(ioopm_iterator_t));

    tracking_list(iter, list);

    return iter;
}


ioopm_iterator_t *
ioopm_array_iterator(array_t *array)
{
    ioopm_iterator_t *iter = calloc(1, sizeof(ioopm_iterator_t));

    tracking_array(iter, array);
    
    return iter;
}

static
bool
iterator_list_set(ioopm_iterator_t *iter, short command)
{
if(!(siz(iter)))
    return false;

switch(command){
    case STAY: 
    break;

    case START:
    iter->index = 0;    
    set_first(iter);
    break;

    case LAST:
    iter->index = siz(iter) - 1;
    set_last(iter);
    break;

    default:
    assert(false);
}
return true;
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
iterator_init(ioopm_iterator_t *iter)
{
    assert(iter);
    //check if empty
    if(!(siz(iter)))
        return false;
 
    if(!(iter->dummied))
        return true;
   
    //Set to start if it was dummied 
    iterator_list_set(iter, START);
    iter->dummied = false;

    return true;
}



void 
ioopm_iterator_destroy(ioopm_iterator_t *iter)
{
    ioopm_filter_all(iter->iterator_list, ioopm_equals_adress, iter); 
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

    if(ioopm_iterator_prev(iter))
        return;

    if(!ioopm_iterator_next(iter))
        iter->dummied = true;
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
index_within(ioopm_iterator_t *iter, short dir)
{
    if(within(0, i_next(iter, dir), end_p(iter)))
        return true;
    return false;
}




static
void
move_c_adress_list(ioopm_iterator_t *iter, short dir)
{
    if(dir == 0)
        return;

    assert(at(iter));

    if(dir > 0)
    {
        at(iter) = node(iter)->next; 
        iter->index++;
        dir--;
        return move_c_adress_list(iter, dir);
    } 
    
    if(dir < 0)
    {
        iter->c_adress = node(iter)->previous; 
        iter->index--;
        dir++;
        return move_c_adress_list(iter, dir);
    }
}

/**
 * @brief Moves c_adress based on type and linear direction
 * 
 * @param type 
 * @param c_adress 
 * @param data 
 * @param dir 
 */
bool
ioopm_move_c_adress(ioopm_iterator_t *iter, short dir)
{
    ioopm_iterator_can_jump(iter, dir);
    if(!index_within(iter, dir) || !iterator_init(iter))
        return false;
    
    switch(iter->type){
        case list_iter:
        move_c_adress_list(iter, dir); 
        break;

        case array_iter:
            //Does this jump right amount?
            at(iter) = at(iter) + dir * chunk(iter);
            iter->index += dir;
                            
        break;
        default:
        assert(false);
    }
    return true;
}



bool
ioopm_iterator_can_jump(ioopm_iterator_t *iter, int distance)
{
    if(!index_within(iter, distance) || !iterator_init(iter))
        return false;
    return true
}

bool 
iterator_has_next(ioopm_iterator_t *iter)
{
    return ioopm_iterator_can_jump(iter, 1);
}

bool 
iterator_has_prev(ioopm_iterator_t *iter)
{
    return ioopm_iterator_can_jump(iter, -1);
}

/**
 * @brief Find smallest of 
 * 
 * @param c_to_g returns STAY 
 * @param s_to_g returns START
 * @param e_to_g returns LAST
 * @return short 
 */
static
short
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
            int temp = siz(iter);
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
        return ioopm_iterator_jump(iter, index - i_next(iter, 0));

        default:
        assert(false);
    }
}

bool
ioopm_iterator_jump(ioopm_iterator_t *iter, int distance)
{
    if(!ioopm_iterator_can_jump(iter, distance))
        return false;
    
    if(iter->type == list_iter)
        ioopm_iterator_set(iter, i_next(iter, distance));
    
    return ioopm_move_c_adress(iter, distance);
}

bool
ioopm_iterator_next(ioopm_iterator_t *iter)
{
    return ioopm_iterator_jump(iter, 1);
}

bool 
ioopm_iterator_prev(ioopm_iterator_t *iter)
{   
    return ioopm_iterator_jump(iter, -1); 
}

option_t
ioopm_iterator_edit(ioopm_iterator_t *iter, 
                    ioopm_transform_value transformation, 
                    void *arg)
{
    if(!iterator_init(iter))
        return garbage;
    switch(iter->type){
        case list_iter:
        {
            //we need to free data we replace..
            //how do we edit key
            return ioopm_edit_node_value(list(iter), transformation, 
                                         node(iter), arg);
            break;
        }

        case array_iter:
        {
            elem_t edit = e(arg);
            if(transformation)
            {
                edit = *(e_p(at(iter)));
                transformation(&edit, arg);
            }
            memcpy(at(iter), &edit, chunk(iter));
            return ioopm_iterator_current_value(iter);
        }

        default:
        assert(false);
    }

}

bool 
ioopm_iterator_remove(ioopm_iterator_t *iter)
{
    switch (iter->type) {
        case list_iter:
            if(!iterator_init(iter)) 
                return false;

            return ioopm_remove_node(list(iter), node(iter)).success;

        case array_iter:
        {
            memset(at(iter), 0, chunk(iter));
            return ioopm_iterator_next(iter);
        }

        default:
        assert(false);
    }
}

void 
ioopm_iterator_reset(ioopm_iterator_t *iter)
{
    ioopm_iterator_set(iter, 0);
}

void
ioopm_iterator_last(ioopm_iterator_t *iter)
{
    ioopm_iterator_set(iter, ioopm_iter_db_siz(iter) - 1);
}

option_t 
ioopm_iterator_current_value(ioopm_iterator_t *iter)
{
    option_t result = {0};
    if(iterator_init(iter))
        return result;
    
    switch (iter->type) {
        case list_iter:
        result.return_value = node(iter)->value;
        break;

        case array_iter:
        result.return_value = node(iter)->value;
        break;


        default:
        assert(false);
    }
    result.success = 1;
    return result;
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
        return ioopm_linked_list_size(list(iter));
        
        case array_iter:
        return array(iter)->elements; 

        default:
        assert(false);
    }
}
void
ioopm_iter_destroy_db(ioopm_iterator_t *iter)
{   
    switch (iter->type) {
        case list_iter:
        ioopm_linked_list_clear(list(iter));
        ioopm_linked_list_destroy(list(iter));
        break;

        case array_iter:
        ioopm_array_destroy(array(iter));
        break;

        default:
        assert(false);
    }
}



//ONLY LIST
static
option_t
iterator_list_add(ioopm_iterator_t *iter, elem_t value, elem_t key, short dir)
{
    option_t result = {0};
    if(iter->type == array_iter)
        return result;
    
    //If left node previous to insert should be previous of this node
    if(dir == LEFT)
        ioopm_iterator_prev(iter);
 
    result = ioopm_insert_node(node(iter), value, key, list(iter));

    //The node inserted is always after previous node
    ioopm_iterator_next(iter);
     
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
    if(!iterator_init(iter)) 
        return garbage;
    
    switch(iter->type){
        case list_iter:
        return option(node(iter)->key, false);

        case array_iter:
        return garbage;

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