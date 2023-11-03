#pragma once
#include "linked_list.h"
#include "array.h"


/// @brief Create an iterator for a given list, a cursor pointing at elements.
/// iterators should be destroyed before a list is destroyed.
/// @param list the list to be iterated over
/// @return an iteration positioned at the start of list
ioopm_iterator_t *
ioopm_list_iterator(ioopm_list_t *list);


/// @brief Creates an iterator over a given array structure
/// @param array Array structure to iterate over
/// @return Pointer to iterator positioned at index 0 
ioopm_iterator_t *
ioopm_array_iterator(array_t *array);



//MOVEMENT

/// @brief Checks if there are more elements to iterate over to right,
/// @param iter
/// @return True if there is at least one more element 
bool 
ioopm_iterator_has_next(ioopm_iterator_t *iter);

/// @brief Checks if there are more elements to iterate over to left,
/// @param iter
/// @return True if there is at least one more element 
bool
ioopm_iterator_has_prev(ioopm_iterator_t *iter);

/**
 * @brief This is the third one, jumps relative to current index
 * //TODO LIST ITERATOR
 * 
 * @param iter 
 * @param distance 
 * @return true  
 * @return false 
 */
bool
ioopm_iterator_can_jump(ioopm_iterator_t *iter, int distance);


/// @brief Step the iterator forward one step, return error if at the end.
/// @param iter the iterator
/// @return True if could move to next
bool 
ioopm_iterator_next(ioopm_iterator_t *iter);

/// @brief Step the iterator backward one step, this will be able
/// to step to the space before the first element so that we can
/// insert with the iterator there!
/// @param iter the iterator
/// @return True if could move to prev
bool
ioopm_iterator_prev(ioopm_iterator_t *iter);

/**
 * @brief 
 * 
 * @param iter 
 * @param distance 
 * @return true 
 * @return false 
 */
bool
ioopm_iterator_jump(ioopm_iterator_t *iter, int distance);



//TRANSFORM ELEMENT
/// @brief Used to edit element iterator is on.
/// If the param is NULL, it means that that param will be nullified
/// @param iter 
/// @param transformation  
/// @param arg In order: 
/// value 
/// key 
/// OR 
/// Args for transformation
/// @return False if db is empty, true else 
/// And the element if list and pointer to element if array
option_t
ioopm_iterator_edit(ioopm_iterator_t *iter, 
                    ioopm_transform_value transformation, 
                    void *arg);


//REMOVE ELEMENT
/// @brief Remove the current element from the underlying list, 
/// jumps to the previous one.
/// @param iter the iterator
/// @return If remove was successful or that was the last to remove
bool 
ioopm_iterator_remove(ioopm_iterator_t *iter);


//VALUE GETTERS

/// @brief Return the current value from the underlying list.
/// @param iter the iterator
/// @return the current value
option_t 
ioopm_iterator_current_value(ioopm_iterator_t *iter);

/// @brief Return the current key from the underlying list.
/// @param iter the iterator
/// @return the current key
option_t 
ioopm_iterator_current_key(ioopm_iterator_t *iter);

//DESTROY ITER
/// @brief Destroy the iterator and return its resources.
/// @param iter the iterator
void 
ioopm_iterator_destroy(ioopm_iterator_t *iter);

/// @brief Destroy...
/// @param iter
/// @return size of...
size_t
ioopm_iter_db_siz(ioopm_iterator_t *iter);


//Location methods

int
ioopm_iter_index(ioopm_iterator_t *iter);

// General db methods
/// @brief Destroy...
/// @param iter
void
ioopm_iter_destroy_db(ioopm_iterator_t *iter);

/// @brief For list this places it as close as possible to the index given
/// For array this just places it at the index
/// @param iter 
/// @param index 
/// @return
/// TRUE if could go to index
/// FALSE if could not go directly to index, 
/// This will still place the iterator as close as possible 
bool
ioopm_iterator_set(ioopm_iterator_t *iter, int index);

/// @brief Reposition the iterator at the start of the underlying list.
/// @param iter the iterator
void 
ioopm_iterator_reset(ioopm_iterator_t *iter);



/// @brief Resets iterator to last
/// @param iter 
void
ioopm_iterator_last(ioopm_iterator_t *iter);


/// @brief Inserts a new value to the right.
/// @param iter the iterator
/// @param value both read and change value
/// @param key read only 
option_t 
ioopm_iterator_insert(ioopm_iterator_t *iter, elem_t value, elem_t key);

/// @brief Inserts a new value to the left.
/// @param iter the iterator
/// @param value both read and change value
/// @param key read only 
option_t 
ioopm_iterator_prepend(ioopm_iterator_t *iter, elem_t value, elem_t key);

//ADVANCED FUNCTIONS
void
ioopm_move_iter_index(ioopm_iterator_t *iter, int index);


/**
 * @brief Powerful but with the wrong info it will totally corrupt
 * the iterators
 * 
 */
void 
ioopm_update_iterators(elem_t *value, void *action_info);