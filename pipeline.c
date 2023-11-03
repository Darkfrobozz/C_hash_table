#include "include/pipeline.h"
#include "include/array.h"
#include "include/nodes.h"

static
bool
assemble_mover(ioopm_iterator_t *iter, elem_t *local_mem)
{

}
static
bool
assemble_remover(ioopm_iterator_t *iter, elem_t *local_mem)
{

}
static
bool
assemble_transformer(ioopm_iterator_t *iter, elem_t *local_mem)
{

}

static
bool
assemble_comparer(ioopm_iterator_t *iter, elem_t *local_mem)
{


}

static
int
assemble_controller(ioopm_iterator_t *iter, elem_t *local_mem, 
                    bool protocol, int index)
{


}
static
elem_t ** 
process_args(elem_t *local_mem, void *args, int amount)
{
    elem_t *type_arr = calloc(amount, sizeof(elem_t));
    elem_t *args_cast = args;

    for(int i = 0; i < amount; i++)
    {
        type_arr[i] = args_cast[i];

        //Put in correct mem location since they share this array
        local_mem[type_arr[i].assemble] = args_cast[i + 1]; 
    }
}

ioopm_iterator_t *
ioopm_run_pipeline(ioopm_iterator_t *iter, void **args, int amount)
{
    if(amount > mem_limit)
        return NULL;
    elem_t local_mem[mem_limit];
    elem_t *type_choices = process_args(local_mem, args, amount);

    bool shared_protocol;
    int index = 0;

    while(true){
        switch (type_choices[index].assemble) {
            case mover:
            shared_protocol = assemble_mover(iter, local_mem);
            break;
            case transformer:
            shared_protocol = assemble_transformer(iter, local_mem);
            break;
            case comparer:
            shared_protocol = assemble_comparer(iter, local_mem);
            break;
            case remover:
            shared_protocol = assemble_remover(iter, local_mem);
            break;
            case custom:
            shared_protocol = ((custom_assemble)local_mem[custom].p)(iter, local_mem); 
            break;
            case controller:
            int index = assemble_controller(iter, local_mem, 
                                            shared_protocol, index);
            break;
            case breaker:
            return iter;
        }
    }
    return NULL; 
}


