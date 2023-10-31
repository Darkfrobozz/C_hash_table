#include "common.h"
#include "iterator.h"

typedef void(*handle_element)(ioopm_iterator_t *iter, void **mem);
typedef bool(*control)(ioopm_iterator_t *iter, void **mem);

/**
 * @brief The idea is to using certain rules go through the list 
 * NOTE that if no ctrl and negative termination -> infinite loop
 * 
 * @param iter Used to go through the list 
 * @param ctrl_before Handles the termination of process, is done before handle! 
 * NULL -> always true  
 * @param handle Handles the side effects of process
 * NULL -> No side effects AND movement = right
 * @param ctrl_after Handles the termination of process, is done after handle! 
 * NULL -> always true
 * @param mem Memory, rules for functions using this is as follows:
 * Three segments, one assigned to each function that it can write and read in
 * It may only read from the other functions mem!!
 * The 1st segment -> ctrl_before mem
 * The 2nd segment -> Handle mem
 * The 3rd segment -> ctrl_after mem
 * Set the right segment to null if you do not wish to read
 * @param steps finally steps, this allows for running pipelines parallel
 * on same cpu
 * Positive -> Loops at most steps times
 * 0 -> exits 
 * Negative -> Goes til ctrl termination 
 * @return ioopm_iterator_t* 
 */
ioopm_iterator_t *
ioopm_pipeline(ioopm_iterator_t *iter, 
               control ctrl_before,
               handle_element handle, 
               control ctrl_after,
               void **mem,
               int steps);


//FOLLOWING METHODS TELLS PIPE TO STOP WHEN INDEX IS REACHED
//THUS LAST ACTION IS TAKEN ON INDEX
//IF 
bool
ioopm_to_index(ioopm_iterator_t *iter, void **mem);

bool
ioopm_to_last(ioopm_iterator_t *iter, void **mem);

bool
ioopm_to_first(ioopm_iterator_t *iter, void **mem);