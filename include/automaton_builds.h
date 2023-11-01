#include "automaton.h"
//REMEMBER TO CLEAN UP INSTRUCTION SETS!
//Ioopm_list_clear


//Remember that the starting position of the iterator used
//with instruction set will change outcome
ioopm_list_t *
ioopm_clear_til_last();


/**
 * @brief 
 * 
 * @param t_func null -> replace 
 * @param arg 
 * Replace -> first arg and replaces value with it
 * else arg is for t_func
 * @return ioopm_list_t* 
 */
ioopm_list_t *
ioopm_transform_til_last(ioopm_transform_value t_func, void **arg);


/**
 * @brief 
 * 
 * @param comparer
 * Uses cmp_only and moves til this cmp_only returns true
 * STOPS AT cmp_returns true
 * @param arg 
 * void * cmp func args
 * void * cmp values = NULL, else cmp keys.
 * 
 * @return ioopm_list_t* 
 */
ioopm_list_t *
ioopm_until_true(ioopm_pred_value comparer, void **arg);


/**
 * @brief 
 * 
 * @param comparer 
 * @param arg 
 * void * cmp func args
 * void * cmp values = NULL, else cmp keys.
 * @return ioopm_list_t* 
 */
ioopm_list_t *
ioopm_filter_til_last(ioopm_pred_value comparer, void **arg);