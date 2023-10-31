#pragma once
#include "common.h"
#include "iterator.h"

typedef bool(*assemblers)(ioopm_iterator_t *iter, 
                          ioopm_iterator_t *assembly_iter,
                          bool prev_result);

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
ioopm_pipeline(ioopm_iterator_t *iter, 
               ioopm_list_t *assembly_list);


//Assembler_control
//Assembler_cast continue
/**
 * @brief Used to continue down the chain of assemblers
 * If u want to accept a next queue you should use this instead of typecasting
 * Make sure to move the a_iter first! 
 * @param iter 
 * @param a_iter 
 * @return true 
 * @return false 
 */
bool
ioopm_assemble_continue(ioopm_iterator_t *iter, 
                        ioopm_iterator_t *a_iter, bool prev);


/**
 * @brief Branches based on previous
 * THIS CAN BE USED TO FORM THE PIPELINE
 * Backwards branching is possible
 * @param iter 
 * @param a_iter 
 * Args:
 * index1: Move to this index if true
 * index2: Move to this index if false
 * @param prev 
 * @return true Moves a_iter to some other
 * @return false Moves a_iter to some other
 */
bool
ioopm_assemble_branch(ioopm_iterator_t *iter, 
                      ioopm_iterator_t *a_iter, bool prev);

/**
 * @brief Removes based on pred on both key and value
 * 
 * @param iter 
 * @param a_iter 
 * args:
 * compare func cmp_value
 * elem_t arg_value
 * compare func cmp_key
 * elem_t arg_key
 * @param prev 
 * @return true 
 * @return false 
 */
bool
ioopm_pipe_pred_remove(ioopm_iterator_t *iter, 
                    ioopm_iterator_t *a_iter, bool prev);

//SIMPLE EDIT FUNCTIONS
/**
 * @brief Takes as argument a pointer to
 * {Transform function, int *}
 * 
 * @param iter 
 * @param a_iter 
 * @param prev 
 * @return true 
 * @return false 
 */
bool
ioopm_pipe_transform(ioopm_iterator_t *iter, 
                    ioopm_iterator_t *a_iter, bool prev);

//FOLLOWING METHODS TELLS PIPE TO STOP WHEN INDEX IS REACHED
//THUS LAST ACTION IS TAKEN ON INDEX
//SIMPLE MOVEMENT METHODS 
bool
ioopm_to_index(ioopm_iterator_t *iter, ioopm_iterator_t *a_iter, bool prev);

bool
ioopm_to_last(ioopm_iterator_t *iter, ioopm_iterator_t *a_iter, bool prev);

bool
ioopm_to_first(ioopm_iterator_t *iter, ioopm_iterator_t *a_iter, bool prev);

bool
ioopm_fast_index(ioopm_iterator_t *iter, 
                 ioopm_iterator_t *a_iter, bool prev);