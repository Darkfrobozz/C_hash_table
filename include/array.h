#include "common.h"
#include "linked_list.h"
typedef struct array array_t;


/**
 * @brief Making a generalized array
 * 
 * @param p_element pointer to element
 * @param element_siz element siz
 * @param array_siz array siz
 * @return array_t* 
 */
array_t *
ioopm_array(elem_t element, size_t element_siz, size_t array_siz);

void
ioopm_array_cleaners(ioopm_transform_value *clean_value);

void
ioopm_array_destroy(array_t *array);