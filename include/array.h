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
ioopm_array(size_t element_siz, size_t array_siz, type_cast caster);

/**
 * @brief  
 * 
 * @param clean_value 
 */
void
ioopm_array_cleaners(array_t *array, ioopm_transform_value clean_value);

/**
 * @brief 
 * 
 * @param array 
 */
void
ioopm_array_destroy(array_t *array);