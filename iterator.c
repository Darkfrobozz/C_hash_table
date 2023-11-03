#include "include/automaton.h"
#include "include/nodes.h"
#include <stdlib.h>
//MACROS
#define node() ((node_t *)(iter->c_adress))
#define list() ((ioopm_list_t *)(iter->d_struct))
#define array() ((array_t *)(iter->d_struct))

#define set_last() ((iter->c_adress) = (list()->last->previous))
#define set_first() ((iter->c_adress) = (list()->first->next))
#define at() (iter->c_adress)
#define cast() (array()->caster(at()))

#define i_next(a) (iter->index + a)
#define siz() (ioopm_iter_db_siz(iter))
#define chunk() (array()->chunk_siz)
#define within(a,x,b) (((x) >= (a)) && ((x) <= (b)))
#define end_p() (siz() - 1)
#define direction(to) (to - i_next(0))

#define init_fail() (!iterator_init(iter))
#define init_or_index_fail(a) (init_fail() || !index_within(iter, a))
#define empty() (!siz())

#define START 1
#define STAY 0
#define LAST -1

enum updates{destroyed, remove_atd, inserted};

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
if(empty())
    return false;

switch(command){
    case STAY: 
    break;

    case START:
    iter->index = 0;    
    set_first();
    break;

    case LAST:
    iter->index = siz() - 1;
    set_last();
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
    if(empty())
        return false;
 
    if(!(iter->dummied))
        return true;
   
    //Set to start if it was dummied 
    if(iterator_list_set(iter, START))
    {
        iter->dummied = false;
        return true;
    }

    return false;
}



void 
ioopm_iterator_destroy(ioopm_iterator_t *iter)
{
    ioopm_filter_all(iter->iterator_list, ioopm_equals_adress, iter); 
}

void
static
remove_at_update(ioopm_iterator_t *iter, void *info)
{
    int index = *((int *)info);
    if(i_next(0) < index)
        return; 
    //If it is empty then we will be dummied no matter what
    if(siz() == 1)
    {
        iter->dummied = true;
        return;
    }
    //If index is bigger then we fall back one block
    if(i_next(0) > index)
        iter->index--;
    else 
    {
        //If the index is equal then the determined behaviour is
        //To fall back one step
        if(prev())
            return;
        //However
        //If we can't go to prev we must go forward or we will be deleted
        next();
    }
}

static
void
insert_update(ioopm_iterator_t *iter, void *info)
{
    int index = *((int *) info);
    if(i_next(0) < index)
        return;
    //If equal then the index is not affected
    if(iter->index == index)
        return;
    //since a node is inserted between the two the index will increase
    //This is because only the end point db_siz is dynamic!!
    //The starting point is static 0
    iter->index++;
}

//This is used in list
//We can create an iterator for pipe
void 
ioopm_update_iterators(elem_t *value, void *action_info)
{
    int *info = action_info;
    enum updates update = info[0];
    action_info++;
    ioopm_iterator_t *iter = value->p;

    switch (update) {
        //This is irrelevant as of now..
        case destroyed:
        return destroy();
        //Logic for sending updates is not established
        case remove_atd:
        return remove_at_update(iter, action_info);
        case inserted:
        return insert_update(iter, action_info);
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
index_within(ioopm_iterator_t *iter, short dir)
{
    if(within(0, i_next(dir), end_p()))
        return true;
    return false;
}




static
void
move_c_adress_list(ioopm_iterator_t *iter, short dir)
{
    if(dir == 0)
        return;

    assert(at());

    if(dir > 0)
    {
        at() = node()->next; 
        iter->index++;
        dir--;
        return move_c_adress_list(iter, dir);
    } 
    
    if(dir < 0)
    {
        iter->c_adress = node()->previous; 
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
    jumpable(dir);
    if(init_or_index_fail(dir))
        return false;
    
    switch(iter->type){
        case list_iter:
        move_c_adress_list(iter, dir); 
        break;

        case array_iter:
            //Does this jump right amount?
            at() = BYTE(at()) + dir * chunk();
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
    if(init_or_index_fail(distance))
        return false;
    return true;
}

bool 
ioopm_iterator_has_next(ioopm_iterator_t *iter)
{
    return jumpable(1);
}

bool 
ioopm_iterator_has_prev(ioopm_iterator_t *iter)
{
    return jumpable(-1);
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
            int temp = siz();
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
        return jump(index - i_next(0));

        default:
        assert(false);
    }
}

bool
ioopm_iterator_jump(ioopm_iterator_t *iter, int distance)
{
    if(!jumpable(distance))
        return false;
    
    if(iter->type == list_iter)
        ioopm_iterator_set(iter, i_next(distance));
    
    return ioopm_move_c_adress(iter, distance);
}

bool
ioopm_iterator_next(ioopm_iterator_t *iter)
{
    return jump(1);
}

bool 
ioopm_iterator_prev(ioopm_iterator_t *iter)
{   
    return jump(-1); 
}

option_t
ioopm_iterator_edit(ioopm_iterator_t *iter, 
                    ioopm_transform_value transformation, 
                    void *arg)
{
    if(init_fail())
        return garbage;
    switch(iter->type){
        case list_iter:
        ioopm_edit_node_value(list(), transformation, 
                              node(), arg);
        return;
        case array_iter:
        {
            //Fetch arguments
            elem_t edit = adress_to_elem(arg);

            if(transformation)
            {
                edit = cast();
                transformation(&edit, arg);
            }

            //Actual replacing happens here!
            memcpy(at(), &edit, chunk());
            return value();
        }

        default:
        assert(false);
    }

}

bool 
ioopm_iterator_remove_at(ioopm_iterator_t *iter)
{
    if(init_fail()) 
        return false;

    switch (iter->type) {
        case list_iter:
        ioopm_list_remove_at(list(), node(), i_next(0));
        return;

        case array_iter:
        {
            memset(at(), 0, chunk());
            return next();
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
ioopm_iterator_value_at(ioopm_iterator_t *iter)
{
    option_t result = {0};
    if(init_fail())
        return result;
    
    switch (iter->type) {
        case list_iter:
        result.return_value = node()->value;
        break;

        case array_iter:
        result.return_value = cast();
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
        return ioopm_linked_list_size(list());
        
        case array_iter:
        return array()->elements; 

        default:
        assert(false);
    }
}
void
ioopm_iter_destroy_db(ioopm_iterator_t *iter)
{   
    switch (iter->type) {
        case list_iter:
        ioopm_linked_list_clear(list());
        ioopm_linked_list_destroy(list());
        break;

        case array_iter:
        ioopm_array_destroy(array());
        break;

        default:
        assert(false);
    }
}



//ONLY LIST
//THIS IS AN IMPORTANT METHOD
static
option_t
iterator_list_add(ioopm_iterator_t *iter, elem_t value, elem_t key, short dir)
{
    option_t result = {0};
    if(iter->type == array_iter)
        return result;
    
    //If left node previous to insert should be previous of this node
    if(dir == LEFT)
    {
        if(!i_next(0))
            ioopm_list_prepend(list(), value, key);
        prev();
    }

    ioopm_list_insert(node(), value, 
                      key, list(), i_next(0));
     
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
ioopm_iterator_key_at(ioopm_iterator_t *iter)
{
    if(init_fail()) 
        return garbage;
    
    switch(iter->type){
        case list_iter:
        return option(node()->key, false);

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
    destroy();
}