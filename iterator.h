#pragma once
#include "linked_list.h"

typedef struct iterator ioopm_iterator_t;

/// @brief Create an iterator for a given list, a cursor pointing at elements.
/// iterators should be destroyed before a list is destroyed.
/// @param list the list to be iterated over
/// @return an iteration positioned at the start of list
ioopm_iterator_t *ioopm_list_iterator(ioopm_list_t *list);

/// @brief Checks if there are more elements to iterate over,
/// checks if the node has a next, if and checks if that next also has a next then there is more non dummy nodes.
/// @param iter the iterator
/// @param success whether the node and list exists or not
/// @return true if there is at least one more element 
bool ioopm_iterator_has_next(ioopm_iterator_t *iter);

/// @brief Step the iterator forward one step, return error if at the end
/// @param iter the iterator
/// @param success whether the node and list exists or not
/// @return the next element
option_t ioopm_iterator_next(ioopm_iterator_t *iter);

/// @brief Step the iterator backward one step, this will be able
/// to step to the space before the first element so that we can
/// insert with the iterator there!
/// @param iter the iterator
/// @param success whether the node and list exists or not
/// @return the previous element
option_t ioopm_iterator_previous(ioopm_iterator_t *iter);


/// NOTE: REMOVE IS OPTIONAL TO IMPLEMENT 
/// @brief Remove the current element from the underlying list, jumps to the previous one
/// @param iter the iterator
/// @param success whether the node and list exists or not
/// @return the removed element
option_t ioopm_iterator_remove(ioopm_iterator_t *iter);

/// NOTE: INSERT IS OPTIONAL TO IMPLEMENT 
/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator
/// @param element the element to be inserted
/// @param success whether the node and list exists or not
option_t ioopm_iterator_insert(ioopm_iterator_t *iter, elem_t element, elem_t key);

/// @brief Reposition the iterator at the start of the underlying list
/// @param iter the iterator
/// @param success Whether list exists.
void ioopm_iterator_reset(ioopm_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @param success whether the node and list exists or not
/// @return the current element
option_t ioopm_iterator_current(ioopm_iterator_t *iter);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
/// @param success whether the node and list exists or not
void ioopm_iterator_destroy(ioopm_iterator_t *iter);

option_t ioopm_iterator_current_key(ioopm_iterator_t *iter);