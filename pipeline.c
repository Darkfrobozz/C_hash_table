#include "pipeline.h"

ioopm_iterator_t *
ioopm_pipeline(ioopm_iterator_t *iter, 
               handle_element handle, void *handle_mem, 
               control ctrl, void *ctrl_mem, int steps)
{
    int counter = 0;
    bool ctrl_bool;
    do if(steps != 0)
    {
        //ctrl is established before action!
        if(ctrl)
            ctrl_bool = ctrl(iter, ctrl_mem);
        else
            ctrl_bool = true;
        //side effects
        if(handle)
            handle(iter, handle_mem);
        else
            ioopm_iterator_next(iter);
        counter++;
    } while ((ctrl_bool && (counter < steps)) 
    || (ctrl_bool && (steps < 0)));
    return iter;
}