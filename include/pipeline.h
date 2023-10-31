#include "common.h"
#include "iterator.h"

typedef void(*handle_element)(ioopm_iterator_t *iter, void *mem);
typedef bool(*control)(ioopm_iterator_t *iter, void *mem);

/**
 * @brief The idea is to using certain rules go through the list 
 * NOTE that if no ctrl and negative termination -> infinite loop
 * 
 * @param iter Used to go through the list 
 * @param handle Handles the side effects of process, is done after ctrl!
 * NULL -> No side effects AND movement = right
 * @param handle_mem Memory to allow for more generalized actions
 * @param ctrl Handles the termination of process, is done before handle! 
 * NULL -> always true 
 * @param ctrl_mem Memory to allow for more generalized controls 
 * @param steps finally steps, this allows for running pipelines parallel
 * on same cpu
 * Positive -> Loops at most steps times
 * 0 -> exits 
 * Negative -> Goes til ctrl termination 
 * @return ioopm_iterator_t* 
 */
ioopm_iterator_t *
ioopm_pipeline(ioopm_iterator_t *iter, 
               handle_element handle, void *handle_mem, 
               control ctrl, void *ctrl_mem, int steps);