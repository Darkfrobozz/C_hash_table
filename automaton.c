#include "include/auto_common.h"
#include "array.h"
#include "nodes.h"

ioopm_iterator_t *
ioopm_run_automaton(ioopm_iterator_t *iter, 
                    am_t *automat)
{
    if(automat->states == 0)
        return iter;
    
    array_t arr;
    ioopm_array_set(&arr, automat->s_amounts, 
                    sizeof(elem_t), (elem_t *) &(automat->states));
    
    ioopm_iterator_t *assembly_iter = ioopm_array_iterator(&arr);
    //Go to top of assemble line at end of each loop
    while(ioopm_assemble_continue(iter, assembly_iter, false))
        ioopm_iterator_reset(assembly_iter);

    ioopm_iterator_destroy(assembly_iter);
    return iter;
}
//states and index
bool
ioopm_linear_continue(ioopm_iterator_t *iter, 
                      ioopm_iterator_t *a_iter, bool prev)
{
    if(ioopm_iterator_next(a_iter).success)
        return ioopm_assemble_continue(iter, a_iter, prev);

    return prev;
}


// Remove all 
bool
ioopm_pipe_remover(ioopm_iterator_t *iter, 
                    ioopm_iterator_t *a_iter, bool prev)
{
    option_t result = ioopm_iterator_remove(iter);

    //Go to next in assembly line
    return ioopm_linear_continue(iter, a_iter, result.success);
    
}

//transform ALL
bool
ioopm_pipe_transform(ioopm_iterator_t *iter, 
                    ioopm_iterator_t *a_iter, bool prev)
{    
    void **args = ioopm_iterator_current_value(a_iter).return_value.p; 

    ioopm_transform_value t_func = args[0];
    ioopm_iterator_edit(iter, t_func, args[1]);

    //Go to next in assembly line
    return ioopm_linear_continue(iter, a_iter, prev);
}

//Simple pipe movement
bool
ioopm_fast_index(ioopm_iterator_t *iter, 
                 ioopm_iterator_t *a_iter, bool prev)
{
    ioopm_iterator_set(iter, ioopm_iterator_current_value(a_iter).return_value.i);
    return ioopm_to_index(iter, a_iter, prev);
}

bool
ioopm_to_index(ioopm_iterator_t *iter, 
               ioopm_iterator_t *a_iter, bool prev)
{
    int c_index = ioopm_iter_index(iter);
    int g_index = ioopm_iterator_current_value(a_iter).return_value.i;
    bool result = false;

    if(g_index < 0 || g_index > ioopm_iter_db_siz(iter))
        return false;
    
    //Remember to continue assemble for last
    if(c_index ==  g_index)
        return ioopm_linear_continue(iter, a_iter, false);

    //We need to stop here, if first fails, it does not do next so we continue
    if(c_index < g_index && ioopm_iterator_next(iter).success)
            result = true;
    else if(ioopm_iterator_previous(iter).success)
            result = true;
    
    return ioopm_linear_continue(iter, a_iter, result);
}

bool
ioopm_to_last(ioopm_iterator_t *iter, 
              ioopm_iterator_t *a_iter,
              bool prev)
{
    int last = ioopm_iter_db_siz(iter) - 1;
    ioopm_iterator_edit(a_iter, NULL, &last);
    return ioopm_to_index(iter, a_iter, false);
}

bool
ioopm_to_first(ioopm_iterator_t *iter, 
               ioopm_iterator_t *a_iter,
               bool prev)
{
    int first = 0;
    ioopm_iterator_edit(a_iter, NULL, &first);
    return ioopm_to_index(iter, a_iter, false);
}

bool
ioopm_assemble_continue(ioopm_iterator_t *iter, 
                        ioopm_iterator_t *a_iter,
                        bool prev)
{
    elem_t a_elem = ioopm_iterator_current_key(a_iter).return_value;
    transition a_func = (transition) a_elem.p;
    assert(a_func);
    return a_func(iter, a_iter, prev);
}

bool
ioopm_assemble_branch(ioopm_iterator_t *iter, 
                      ioopm_iterator_t *a_iter, bool prev)
{
    void **arg = ioopm_iterator_current_value(a_iter).return_value.p;
    int *move_true = arg[0];
    int i_true =  ioopm_iter_index(a_iter) + *move_true;
    int *move_false = arg[1];
    int i_false =  ioopm_iter_index(a_iter) + *move_false;
    //If gets true move to true value
    if(prev)
    {
        if(*move_true == 0)
            return false;
        //This creates a new list that must be destroyed!!
        ioopm_move_iter_index(a_iter, i_true);
        return ioopm_assemble_continue(iter, a_iter, prev);
    }

    if(*move_false == 0)
        return false;
    
    ioopm_move_iter_index(a_iter, i_false);
    return ioopm_assemble_continue(iter, a_iter, prev);
}

bool
ioopm_assemble_comparer(ioopm_iterator_t *iter,
                        ioopm_iterator_t *a_iter, bool prev)
{
    void **arg = ioopm_iterator_current_value(a_iter).return_value.p;
    ioopm_pred_value cmp = arg[0]; 
    option_t key_result = ioopm_iterator_current_value(iter);
    option_t value_result = ioopm_iterator_current_value(iter);
    bool result = false;

    if(arg[3])
        prev = true;

    if(arg[2] && key_result.success && cmp 
       && cmp(key_result.return_value, arg[1]) && prev) 
            result = true;
    else if(value_result.success && cmp 
            && cmp(value_result.return_value, arg[1]) && prev) 
                result = true;
    
    return ioopm_linear_continue(iter, a_iter, result);
}

bool
ioopm_assemble_NOT(ioopm_iterator_t *iter,
                   ioopm_iterator_t *a_iter, bool prev)
{
    void *arg = ioopm_iterator_current_value(a_iter).return_value.p;
    if(arg)
        return !prev;
    return ioopm_linear_continue(iter, a_iter, !prev);
}