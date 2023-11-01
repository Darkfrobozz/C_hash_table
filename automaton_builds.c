#include "include/automaton_builds.h"

ioopm_list_t *
ioopm_clear_til_last()
{
    ioopm_list_t *assembly_list = ioopm_linked_list_create();
    ioopm_linked_list_append(assembly_list, (elem_t) (void *) 0, 
                           (elem_t) (void *) ioopm_pipe_remover);
    return assembly_list;
}

ioopm_list_t *
ioopm_transform_til_last(ioopm_transform_value t_func, void **arg)
{
    ioopm_list_t *assembly_list = ioopm_linked_list_create();
    void *inc_arg[] = {t_func, *arg};
    ioopm_linked_list_append(assembly_list, 
                            (elem_t) (void *) inc_arg, 
                            (elem_t) (void *) ioopm_pipe_transform);
    return assembly_list; 
}


ioopm_list_t *
ioopm_until_true(ioopm_pred_value comparer, void **arg)
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
    return assembly_list;
}


ioopm_list_t *
ioopm_filter_til_last(ioopm_pred_value comparer, void **arg)
{
    return NULL;
}