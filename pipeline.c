#include "pipeline.h"

ioopm_iterator_t *
ioopm_pipeline(ioopm_iterator_t *iter, 
               control ctrl_before,
               handle_element handle, 
               control ctrl_after,
               void **mem,
               int steps)
{
    int counter = 0;
    //Controls that list is not empty, empty lists are never allowed to pipe
    size_t siz = ioopm_iter_db_siz(iter);
    if(!siz)
        return iter;

    for (int counter = 0; counter < steps || steps < 0; counter++){
        //ctrl is established before action!
        if(ctrl_before && ctrl_before(iter, mem))
            break;
        //side effects
        if(handle)
            handle(iter, mem);
        else
            ioopm_iterator_next(iter);
        if(ctrl_after && ctrl_after(iter, mem))
            break;
    } 

    return iter;
}

bool
ioopm_to_index(ioopm_iterator_t *iter, void **mem)
{
    int c_index = ioopm_iter_index(iter);
    int *g_index = (int *) mem;

    if(c_index ==  *g_index)
        return true;
    
    return false;
}

bool
ioopm_to_last(ioopm_iterator_t *iter, void **mem)
{
    int last = ioopm_iter_db_siz(iter) - 1;
    return ioopm_to_index(iter, &last);
}

bool
ioopm_to_first(ioopm_iterator_t *iter, void **mem)
{
    int first = 0;
    return ioopm_to_index(iter, &first);
}