#include "include/automaton_builds.h"
#include "include/auto_common.h"



//arg format {{2, 0} }
static
void
clear_til_last(am_t *automat, elem_t *args)
{
    elem_t remove_state[1] = {(elem_t) 0};
    //This needs to move one bac
    elem_t branch_state[3] = {(elem_t) 2,(elem_t) -1,(elem_t) 0};
    elem_t *argsreal[2] = {remove_state, branch_state};
    transition actions[2] = {ioopm_pipe_remover, ioopm_assemble_branch};
    ioopm_automaton_settings(5, automat->states, argsreal, actions, true);
}

// static
// void
// transform_til_last(ioopm_transform_value t_func, void **arg)
// {
//     elem_t *argsreal[2] = {1, 5};
//     ioopm_list_t *assembly_list = ioopm_linked_list_create();
//     void *inc_arg[] = {t_func, *arg};
//     ioopm_linked_list_append(assembly_list, 
//                             (elem_t) (void *) inc_arg, 
//                             (elem_t) (void *) ioopm_pipe_transform);
//     return assembly_list; 
// }

static
void
until_true(ioopm_pred_value comparer, void **arg)
{  
    ioopm_list_t *assembly_list = ioopm_linked_list_create();
    int true_branch = 0;
    int false_branch = 1;
    void *cmp_arg[] = {comparer, arg[0], NULL, arg[1]};
    void *branch_arg[] = {&true_branch, &false_branch};
    ioopm_linked_list_append(assembly_list, (elem_t) (void *) cmp_arg, 
                             (elem_t) (void *) ioopm_assemble_comparer);
    ioopm_linked_list_append(assembly_list, (elem_t) (void *) branch_arg, 
                             (elem_t) (void *) ioopm_assemble_branch);
    ioopm_linked_list_append(assembly_list, (elem_t) (void *) 0, 
                             (elem_t) (void *) ioopm_to_last);
}

static
void
filter_til_last(ioopm_pred_value comparer, void **arg)
{
}


void
ioopm_automaton_destroy(am_arr_t *am_arr)
{
    am_t *automatons = am_arr->simple_automatons;
    for(int i = 0; i < simple_siz; i++)
    {
        state_t *states = automatons[i].states;
        size_t mem_siz = automatons[i].mem_amount;
        free(states);
        for(int j = 0; j < mem_siz; j++)
        {
            if(automatons[i].mem + j)
                free(automatons[i].mem + j);
        } 
    }
    //Potentially clean list here as well
}


state_t *
ioopm_state_builder(mem_clean i_clean, transition i_trans, 
              elem_t *args, size_t siz)
{
    state_t *n_state = calloc(1, sizeof(state_t));
    n_state->m_clean = i_clean;
    n_state->s_transiton = i_trans;
    return n_state;
}

//TODO
bool
ioopm_state_adder(am_t *am, state_t *state, 
            elem_t *args, size_t siz)
{
    //NON ITERABLE ARRAY

    return true;
}


/**
 * @brief Allocates states array and initiates key to point to same mem
 * 
 * @param new 
 * @param states 
 */
void
allocating_automat(am_t *new, size_t states)
{
    new->s_amounts = states;
    new->states = calloc(states, sizeof(state_t));
    for(int i = 0; i < states; i++)
    {
        state_t *new_s = new->states + i;
        new_s->mem_index = i;
        new_s->mem = &(new->mem);
    }
}

am_t *
ioopm_build_automaton(am_arr_t *am, 
                      enum tasks d_job, elem_t *args)
{
    if(am->simple_automatons[d_job].am_task == d_job)
        return am->simple_automatons + d_job;
    
    am_t *new = am->simple_automatons + d_job;
    switch (d_job) {
        case remover:
        allocating_automat(new, 1);
        clear_til_last(new, args);
        break;

        default:
        break;
    }
    return am->simple_automatons + d_job;
}

//arg format array
void
ioopm_automaton_settings(int siz, 
                         state_t *states, elem_t **arg, 
                         transition *trans_arr, bool first_time)
{
    for(int i = 0; i < siz; i++)
    {
        elem_t *currentargs = arg[i];
        size_t chunk_siz = currentargs[0].siz;
        elem_t **mem_pointer = states[i].mem;
        if(!chunk_siz)
            continue;

        if(first_time)
        {
            mem_pointer[i] = calloc(chunk_siz, sizeof(elem_t));
            currentargs++;
            states[i].s_transiton = trans_arr[i];
        }

        memcpy(mem_pointer[i], currentargs, chunk_siz);
    }
}

