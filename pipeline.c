#include "include/pipeline.h"
#include "include/array.h"
#include "include/nodes.h"


static
elem_t * 
process_args(elem_t *local_mem, elem_t *args, int amount)
{
    elem_t *type_arr = calloc(amount, sizeof(elem_t));

    for(int i = 0; i < amount*2; i += 2)
    {
        type_arr[i / 2] = args[i];
        //Put in correct mem location since they share this array
        local_mem[type_arr[i / 2].assemble] = args[i + 1]; 
    }
    return type_arr;
}

ioopm_iterator_t *
ioopm_run_pipeline(ioopm_iterator_t *iter, elem_t *args, int amount)
{
    if(amount > mem_limit)
        return NULL;
    elem_t mem[mem_limit] = {0};
    elem_t *type_choices = process_args(mem, args, amount);

    bool shared_protocol;
    int index = 0;
    
    //Controller must be after each
    while(index < amount && index >= 0){

        void **local_arg = mem[type_choices[index].i].p;

        switch (type_choices[index].assemble) {

            //Used by a lot of functions
            case mover:
            shared_protocol = jump(mem[mover].i);
            break;

            case transformer:
            {
                ioopm_transform_value edit_func = local_arg[0];
                shared_protocol = edit(edit_func, local_arg[1]).success;
                break;
            }
            case cmpvalue:
            {
                ioopm_pred_value cmp_func = local_arg[0];
                shared_protocol = cmp_func(value().return_value, local_arg[1]);
                break;
            }
            case cmpkey:
            {
                ioopm_pred_value cmp_func = local_arg[0];
                shared_protocol = cmp_func(key().return_value, local_arg[1]);
                break;
            }

            case remover:
            shared_protocol = remove_at();
            break;

            case custom:
            shared_protocol = ((custom_assemble)mem[custom].p)(iter, mem); 
            break;

            default:
            break; 
        }

        void **controll_args = mem[controller].p;

        if(controll_args)
        {
            //Notice that pred is also responsible for incrementing index
            ioopm_pred_value to_break = controll_args[0];
            elem_t i_send;
            void *send_args[3] = {type_choices + index, &index, controll_args[1]};
            i_send.b = shared_protocol;
            if(to_break && !to_break(i_send, send_args))
                break; 
        } else if(!shared_protocol)
            break; 

        index++;
        if(index >= amount)
            index = 0;
    }
    free(type_choices);

    return iter; 
}


