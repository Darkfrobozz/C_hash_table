#pragma once
#include "linked_list.h"


typedef struct iterator ioopm_iterator_t;


/// @brief Create an iterator for a given list, a cursor pointing at elements.
/// iterators should be destroyed before a list is destroyed.
/// @param list the list to be iterated over
/// @return an iteration positioned at the start of list
ioopm_iterator_t *
ioopm_list_iterator(ioopm_list_t *list);

/// @brief Checks if there are more elements to iterate over,
/// checks if the node has a next, if and checks if that next also has a next 
/// then there is more non dummy nodes.
/// @param iter the iterator
/// @return true if there is at least one more element 
bool 
ioopm_iterator_has_next(ioopm_iterator_t *iter);

/// @brief
///
/// @param
/// @return
bool
ioopm_iterator_has_prev(ioopm_iterator_t *iter);

/// @brief Step the iterator forward one step, return error if at the end.
/// @param iter the iterator
/// @return the next element
option_t 
ioopm_iterator_next(ioopm_iterator_t *iter);

/// @brief Step the iterator backward one step, this will be able
/// to step to the space before the first element so that we can
/// insert with the iterator there!
/// @param iter the iterator
/// @return the previous element
option_t 
ioopm_iterator_previous(ioopm_iterator_t *iter);

/// @brief Used to edit element iterator is on.
/// If the param is NULL, it means that it won't be changed.
/// This means you can not change it to a null pointer.
/// @param iter 
/// @param transformation  
/// @param arg
void
ioopm_iterator_edit(ioopm_iterator_t *iter, 
                    ioopm_transform_value transformation, 
                    void *arg);

/// @brief Remove the current element from the underlying list, 
/// jumps to the previous one.
/// @param iter the iterator
/// @return the removed element
option_t 
ioopm_iterator_remove(ioopm_iterator_t *iter);

/// @brief Inserts a new value to the right.
/// @param iter the iterator
/// @param element the element to be inserted
/// @param dir whether to insert to the right or left of current node, 
/// note that this is irrelevant in empty list
option_t 
ioopm_iterator_insert(ioopm_iterator_t *iter, elem_t value, elem_t key, 
                      short dir);

/// @brief Reposition the iterator at the start of the underlying list.
/// @param iter the iterator
void 
ioopm_iterator_reset(ioopm_iterator_t *iter);

/// @brief Return the current value from the underlying list.
/// @param iter the iterator
/// @return the current value
option_t 
ioopm_iterator_current_value(ioopm_iterator_t *iter);

/// @brief Destroy the iterator and return its resources.
/// @param iter the iterator
void 
ioopm_iterator_destroy(ioopm_iterator_t *iter);

/// @brief Return the current key from the underlying list.
/// @param iter the iterator
/// @return the current key
option_t 
ioopm_iterator_current_key(ioopm_iterator_t *iter);

/// @brief Clears.
/// @param value 
/// @param arg
void
ioopm_iterator_clear(elem_t *value, void *arg);

/// @brief Destroy...
/// @param iter
void
ioopm_iter_destroy_list(ioopm_iterator_t *iter);

/// @brief Destroy...
/// @param iter
/// @return size of...
size_t
ioopm_iter_list_siz(ioopm_iterator_t *iter);