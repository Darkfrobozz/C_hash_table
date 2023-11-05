#include "include/pipeline.h"
#include "include/array.h"
#include "include/nodes.h"

static
bool
pipe_editing(ioopm_iterator_t *iter, void **arg, node_data choice)
{
    ioopm_transform_value edit_func = arg[0];
    return edit_choice(edit_func, arg[1], choice).success;
}

static
bool
pipe_cmping(ioopm_iterator_t *iter, void **arg, node_data choice)
{
    elem_t to_cmp;
    if(choice == data_value)
        to_cmp = value().return_value;
    else
        to_cmp = key().return_value;
    
    ioopm_pred_value cmp_func = arg[0];

    if(!cmp_func)
        return true;

    if(cmp_func(to_cmp, arg[1]))
        return true;
    return false;
}

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

            case transform_values:
            shared_protocol = pipe_editing(iter, local_arg, data_value);
            break;

            case transform_keys:
            shared_protocol = pipe_editing(iter, local_arg, data_key);
            break;

            case cmpvalue:
            shared_protocol = pipe_cmping(iter, local_arg, data_value);
            break;

            case cmpkey:
            shared_protocol = pipe_cmping(iter, local_arg, data_key);
            break; 

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

static
bool
until_true(elem_t value, void *arg)
{
  void **args = arg;
  enum pipe_assemblers *assemble = args[0];
  switch (*assemble) {
    case mover:
    return value.b;
    case controller:
    return true;
    default:
    return !value.b;
  }
}

bool
ioopm_move_until_true(ioopm_iterator_t *iter, node_data choice, 
                      ioopm_pred_value pred, void **extra)
{
    if(empty())
        return false;
    elem_t s_cmp;

    if(choice == data_key)
        s_cmp.assemble = cmpkey;
    else
        s_cmp.assemble = cmpvalue;

    elem_t s_mv = {.assemble = mover};
    elem_t s_ctrl = {.assemble = controller};
    void *cmp_arg[] = {pred, extra};
    void *ctrl_arg[] = {until_true, NULL};
    elem_t s_ctrl_arg = {.p = ctrl_arg};
    elem_t s_cmp_arg = {.p = cmp_arg};

    //Assembly line:
    elem_t line[] = {s_cmp, s_cmp_arg, s_mv, (elem_t) 1, s_ctrl, s_ctrl_arg};

    ioopm_run_pipeline(iter, line, 3);

    if(!has_next())
    {
        if(choice == data_key && (!pred || pred(key().return_value, extra)))
            return true;
        else if(!pred || pred(value().return_value, extra))
            return true;
        return false;
    }

    return true;
}

void
ioopm_transform_all(ioopm_iterator_t *iter, node_data choice, 
                    ioopm_transform_value t_func, void *extra)
{
    elem_t s_mv = {.assemble = mover};
    elem_t s_edit;

    if(choice == data_value)
        s_edit.assemble = transform_values;
    else
        s_edit.assemble = transform_keys; 

    void *edit_extras[] = {t_func, extra};
    elem_t s_edit_extras = {.p = edit_extras};

    elem_t line[] = {s_edit, s_edit_extras, s_mv, (elem_t) 1}; 

    ioopm_run_pipeline(iter, line, 2);
}