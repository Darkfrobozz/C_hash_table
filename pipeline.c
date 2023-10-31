#include "include/pipeline.h"
#include <stdio.h>

ioopm_iterator_t *
ioopm_pipeline(ioopm_iterator_t *iter, 
               ioopm_list_t *assembly_list)
{
    //Does not do anything if no assemblers here
    if(!assembly_list || ioopm_linked_list_is_empty(assembly_list))
     return iter;

    ioopm_iterator_t *assembly_iter = ioopm_list_iterator(assembly_list);
    //Go to top of assemble line at end of each loop
    while(ioopm_assemble_continue(iter, assembly_iter, false))
        ioopm_iterator_reset(assembly_iter);

    ioopm_iterator_destroy(assembly_iter);
    return iter;
}


// Remove all if
bool
ioopm_pipe_pred_remove(ioopm_iterator_t *iter, 
                    ioopm_iterator_t *a_iter, bool prev)
{
    void **args = ioopm_iterator_current_value(a_iter).return_value.p;
    compare_func cmp_value = args[0];
    elem_t *a_value = args[1];
    compare_func cmp_key = args[2];
    elem_t *a_key = args[3];
    prev = false;
    elem_t current_value = ioopm_iterator_current_value(iter).return_value;
    elem_t current_key = ioopm_iterator_current_key(iter).return_value;

    if(cmp_value 
       && cmp_value(current_value, *a_value))
    {
        prev = ioopm_iterator_remove(iter).success;
    }
    else if(cmp_key 
            && cmp_key(current_key, *a_key))
        {
            prev = ioopm_iterator_remove(iter).success;
        }

    //Go to next in assembly line
    if(ioopm_iterator_next(a_iter).success)
        return ioopm_assemble_continue(iter, a_iter, prev);

    return prev;
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
    if(ioopm_iterator_next(a_iter).success)
        return ioopm_assemble_continue(iter, a_iter, prev);

    return prev;
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
    {
        if(ioopm_iterator_next(a_iter).success)
            return ioopm_assemble_continue(iter, a_iter, false); 
        return false;
    }

    //We need to stop here, if first fails, it does not do next so we continue
    if(c_index < g_index && ioopm_iterator_next(iter).success)
            result = true;
    else if(ioopm_iterator_previous(iter).success)
            result = true;
    
    if(ioopm_iterator_next(a_iter).success)
        return ioopm_assemble_continue(iter, a_iter, result);
    
    return result;
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
    assemblers a_func = (assemblers) a_elem.p;
    assert(a_func);
    return a_func(iter, a_iter, prev);
}

bool
ioopm_assemble_branch(ioopm_iterator_t *iter, 
                      ioopm_iterator_t *a_iter, bool prev)
{
    void **arg = ioopm_iterator_current_value(a_iter).return_value.p;
    int *i_true = arg[0];
    int *i_false = arg[1];
    if(prev)
    {
        if(*i_true < 0)
            return false;
        //This creates a new list that must be destroyed!!
        ioopm_move_iter_index(a_iter, *i_true);
        return ioopm_assemble_continue(iter, a_iter, prev);
    }

    if(*i_false < 0)
        return false;
    
    ioopm_move_iter_index(a_iter, *i_false);
    return ioopm_assemble_continue(iter, a_iter, prev);
}
