#include "include/nodes.h"
#include "include/pre_alloced.h"

// Pre_alloc is a struct containing the index, of where next to be inserted
//It also contains a list to dynamically remove pieces and be able to reuse these parts 
//Note that it is allocated with the structed!
struct pre_alloc {
    enum pre_alloc_types type;
    size_t size;
    void *d_struct;
    int index;
    ioopm_list_t dynamic_list; 
};

pre_alloc_t *
ioopm_gen_pre_alloc(size_t siz, enum pre_alloc_types type)
{
    pre_alloc_t *result = calloc(1, sizeof(pre_alloc_t));
    result->size = siz;
    result->type = type;
    ioopm_prep_list(&result->dynamic_list);
    switch (type) {

        case (list_alloc):
        result->d_struct = calloc(siz, sizeof(ioopm_list_t));
        break;

        case (node_alloc):
        result->d_struct = calloc(siz, sizeof(node_t));        
        break;

        default:
        assert(0);

    }
    return result;
}


bool
ioopm_destruct_pre_alloc(pre_alloc_t *pre_alloc)
{
    ioopm_linked_list_clear(&pre_alloc->dynamic_list);
    free(pre_alloc->d_struct);
    free(pre_alloc);
    return true;
}


option_t
ioopm_pre_alloc_get(pre_alloc_t *alloc_s)
{
    option_t result = {0};
    int choice = -1;

    if(alloc_s->index < alloc_s->size)
        choice = alloc_s->index;

    result = ioopm_linked_list_remove(&alloc_s->dynamic_list, 0);

    if(result.success)
        choice = result.return_value.i; 

    if(choice == -1)
        return (option_t) {0};
    
    switch (alloc_s->type) {

        case (list_alloc):
        {
            ioopm_list_t *lists = alloc_s->d_struct;
            ioopm_prep_list(lists + choice);
            result.return_value.p = lists + choice;
            break;
        }

        case (node_alloc):
        {
            node_t *nodes = alloc_s->d_struct;
            result.return_value.p = nodes + choice;
            break;
        }
    }
    alloc_s->index++;
    return result;
}

option_t
ioopm_remove_alloc_slot(pre_alloc_t *alloc_s, int index)
{
    elem_t i;
    option_t result = {0};
    i.i = index;

    //It has to be within index used and not already be removed
    if(index >= alloc_s->index 
       || ioopm_linked_list_contains(&alloc_s->dynamic_list, i, ioopm_equals_int))
        return result;

    return ioopm_linked_list_append(&alloc_s->dynamic_list, i, (elem_t) 0);
}