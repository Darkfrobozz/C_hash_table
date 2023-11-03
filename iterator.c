#include "include/pipeline.h"
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
#define siz() ((int)(ioopm_iter_db_siz(iter)))
#define chunk() (array()->chunk_siz)
#define within(a,x,b) (((x) >= (a)) && ((x) <= (b)))
#define end_p() (siz() - 1)
#define direction(to) (to - i_next(0))
#define init_fail(a) (!(iter_init(iter, a)))
#define empty() (!siz())
#define force_list_move(a) move_c_adress_list(iter, a)

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
    ioopm_list_t *iterator_list;
};



static
void
tracking_list(ioopm_iterator_t *iter, ioopm_list_t *list)
{
    iter->type = list_iter;
    iter->d_struct = list;
    iter->c_adress = list->first;
    iter->index = -1;
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

void 
ioopm_iterator_destroy(ioopm_iterator_t *iter)
{
    ioopm_filter_all(iter->iterator_list, ioopm_equals_adress, iter); 
}


//FORCED MOVEMENT
static
void
move_c_adress_list(ioopm_iterator_t *iter, short dir)
{
    if(dir == 0)
        return;

    assert(at());

    if(dir > 0 && i_next(0) > -1)
    {
        at() = node()->next; 
        iter->index++;
        dir--;
        return move_c_adress_list(iter, dir);
    } 
    
    if(dir < 0 && i_next(0) < siz())
    {
        iter->c_adress = node()->previous; 
        iter->index--;
        dir++;
        return move_c_adress_list(iter, dir);
    }
}

void
static
remove_at_update(ioopm_iterator_t *iter, int index)
{
    //Check if we need to move iterator
    if(i_next(0) == index) 
        if(!prev())
            if(!next())
                move_c_adress_list(iter, -1);
    
    //We fall back one index if the index is bigger and
    //all that equal are pushed away from node in question
    if(i_next(0) > index)
        iter->index--; 
}

static
void
insert_update(ioopm_iterator_t *iter, int index)
{
    if(i_next(0) == -1)
    {
        reset();
        return;
    }

    if(i_next(0) <= index)
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
    int **args = action_info; 
    enum updates update = *(args[0]);
    action_info++;
    ioopm_iterator_t *iter = value->p;

    switch (update) {
        //This is irrelevant as of now..
        case destroyed:
        return destroy();
        //Logic for sending updates is not established
        case has_removed:
        return remove_at_update(iter, *(args[1]));
        case inserted:
        return insert_update(iter, *(args[1]));
    }
}


/**
 * @brief This is to initialize and ctrl movement
 * 
 * @param iter 
 * @param dir 
 * @return true 
 * @return false 
 */
static
bool
iter_init(ioopm_iterator_t *iter, short dir)
{
    //If it is not in bounds, we should be resetting here
    if(!within(0, i_next(0), end_p()))
        reset();
    
    //If empty this always be false and since we reset
    //We should now be able to use the actual direction commanded
    //To properly evaluate whether we can jump there
    if(!within(0, i_next(dir), end_p()))
        return false;

    return true;
}
/**
 * @brief Moves c_adress based on type and linear direction
 * This is the public version of move_c_adress
 * 
 * @param type 
 * @param c_adress 
 * @param data 
 * @param dir 
 */
bool
ioopm_move_iterator(ioopm_iterator_t *iter, short dir)
{
    if(init_fail(dir))
        return false;

    switch(iter->type){
        case list_iter:
        move_c_adress_list(iter, dir); 
        break;

        case array_iter:
        at() = BYTE(at()) + dir * chunk();
        iter->index += dir;
                            
        break;
        default:
        assert(false);
    }

    return true;
}


//This is just for public convenience!
bool
ioopm_iterator_can_jump(ioopm_iterator_t *iter, int distance)
{
    if(init_fail(distance))
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
 * @brief There are three ways to set the cursor in
 * O(1) time, this abuses that
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
    
    int temp = iter->index;
    //This is to do fast jumps abusing the doubly 
    //linked nature of the list 
    //This is redudant if distance is lower than 1
    if(iter->type == list_iter && distance > 1)
        ioopm_iterator_set(iter, i_next(distance));

    //If index is updated by set we need to update distance
    if(temp != i_next(0))
        distance = (temp + distance) - i_next(0); 
    
    //This is what actually moves this bad boi
    return ioopm_move_iterator(iter, distance);
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
    //Here it means that we when not even moving are not in the
    //right place
    if(init_fail(0))
        return garbage;

    switch(iter->type){
        case list_iter:
        ioopm_list_edit(list(), transformation, 
                        node(), arg);
        return value();
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
    if(init_fail(0)) 
        return false;

    switch (iter->type) {
        case list_iter:
        ioopm_list_remove_at(list(), node(), i_next(0));
        return true;

        case array_iter:
        {
            memset(at(), 0, chunk());
            return next();
        }

        default:
        assert(false);
    }
}

bool 
ioopm_iterator_reset(ioopm_iterator_t *iter)
{
    return ioopm_iterator_set(iter, 0);
}

bool
ioopm_iterator_last(ioopm_iterator_t *iter)
{
    return ioopm_iterator_set(iter, ioopm_iter_db_siz(iter) - 1);
}

option_t 
ioopm_iterator_value_at(ioopm_iterator_t *iter)
{
    option_t result = {0};
    if(init_fail(0))
        return garbage;
    
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
    if(init_fail(0))
        return 0;
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
    int temp;

    if(iter->type == array_iter)
        return result;
    
    //If left node previous to insert should be previous of this node
    //Forcing backwards movement Notice move_c_adress
    if(dir == LEFT)
        force_list_move(-1);
    
    temp = iter->index;
    ioopm_list_insert(node(), value, 
                      key, list(), i_next(0));
    
    //Handling special case exception
    if(temp == -1)
        return result; 
    

    if(dir == RIGHT)
        next();
    else
        prev();

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
    if(init_fail(0)) 
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
    free(iter);
}