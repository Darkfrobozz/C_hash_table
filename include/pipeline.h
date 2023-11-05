#pragma once
#include "common.h"
#include "iterator.h"

typedef bool(*custom_assemble)(ioopm_iterator_t *iter, elem_t *memory);


/**
 * @brief The idea is to put a list of iters through an assembly
 * line til that assembly line reports back that they need to stop
 * 
 * @param iter Used to go through the list 
 * @param assembly_list list of assemblers, functions that are
 * Used handle the elements in the list 
 * Iter's movement
 * Termination - This implies that the user needs to know how
 * the pipeline will terminate, usually this equates to putting
 * some movement assembler in the list
 * The list keys are its functions THESE CAN ONLY BE READ
 * The lists values is its memory, so if one wants to use a function
 * that needs an integer, you place it in the value part of the assembly
 * list
 * Most loops are prevented since the default is to stop looping
 * 
 * @return ioopm_iterator_t* 
 */

ioopm_iterator_t *
ioopm_run_pipeline(ioopm_iterator_t *iter, elem_t *args, int amount);


bool
ioopm_move_until_true(ioopm_iterator_t *iter, node_data choice, 
                      ioopm_pred_value pred, void **extra);

void
ioopm_transform_all(ioopm_iterator_t *iter, node_data choice, 
                    ioopm_transform_value t_func, void *extra);