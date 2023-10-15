#pragma once
#include <stdbool.h>
#include "common.h"




/// @brief Creates a new empty list
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create(void);

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_linked_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
option_t 
ioopm_linked_list_append(ioopm_list_t *list, elem_t value, elem_t key);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
option_t 
ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value, elem_t key);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted 
option_t 
ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value, elem_t key);

/// @brief Remove an element from a linked list in O(n) time. This complexity
/// will also scale with number of iterators. That is because it has to inform them that
/// an action has been taken otherwise they will not know if their node is removed.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list
/// @param index the position in the list
/// @return the value removed
option_t 
ioopm_linked_list_remove(ioopm_list_t *list, int index);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
option_t 
ioopm_linked_list_get(ioopm_list_t *list, int index);

/// @brief Test if an element is in the list
/// @param list the linked list
/// @param element the element sought
/// @return true if element is in the list, else false
bool 
ioopm_linked_list_contains(ioopm_list_t *list, elem_t element, ioopm_pred_value comparer);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
size_t 
ioopm_linked_list_size(ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool 
ioopm_linked_list_is_empty(ioopm_list_t *list);

/// @brief Remove all elements from a linked list
/// @param list the linked list
void 
ioopm_linked_list_clear(ioopm_list_t *list);

/// @brief Test if a supplied property holds for all elements in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested (function pointer)
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for all elements in the list, else false
bool 
ioopm_linked_list_all(ioopm_list_t *list, ioopm_pred_value prop, void *extra);

/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
bool 
ioopm_linked_list_any(ioopm_list_t *list, ioopm_pred_value prop, void *extra);

/// @brief Apply a supplied function to all elements in a list.
/// @param list the linked list
/// @param fun the function to be applied
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of fun
void 
ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_transform_value fun, void *extra);


void 
ioopm_filter_all(ioopm_list_t *list, ioopm_pred_value pred, void *extra);


void 
ioopm_inform_removal(elem_t *value, void *arg);

option_t 
ioopm_remove_node(ioopm_list_t *iterator_list, node_t *remove_node);

option_t 
ioopm_insert_node(node_t *prev_node, elem_t i_value, elem_t i_key, ioopm_list_t *list);


ioopm_list_t *
ioopm_get_iterator(ioopm_list_t *list);

void
ioopm_add_cleaners(ioopm_list_t *list, ioopm_transform_value i_clean_value, 
                   ioopm_transform_value i_clean_key);


/**
 * @brief ListA is changed to the result. ListB is only read from.
 * 
 * @param listA 
 * @param listB 
 */
void
ioopm_append_lists(ioopm_list_t *listA, ioopm_list_t *listB, bool takekey);




/**
 * @brief Extending the apply all to both keys and values
 * 
 * @param list 
 * @param fun_value 
 * @param extra_value 
 * @param fun_key 
 * @param extra_key 
 */
void 
ioopm_list_apply_extended(ioopm_list_t *list, 
                               ioopm_transform_value fun_value, 
                               void *extra_value,
                               ioopm_transform_value fun_key, 
                               void *extra_key);

elem_t
ioopm_element_to_list(elem_t list, elem_t item);