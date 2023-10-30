#include "include/array.h"
#include "include/nodes.h"
#include "iterator.h"

array_t *
ioopm_array(void *element, size_t element_siz, size_t array_siz)
{
    array_t *new_array_struct = calloc(1, sizeof(array_t));
    void *new_data = calloc(array_siz, element_siz);
    new_array_struct->p_first_element = new_data;
    new_array_struct->chunk_siz = element_siz;
    new_array_struct->chunk_amount = array_siz;
    new_array_struct->iterator_list = ioopm_iterator_list_create();
    return new_array_struct;
}


void
ioopm_array_cleaners(array_t *arr, ioopm_transform_value *clean_value)
{
    arr->clean_value = clean_value;
}


void
ioopm_array_destroy(array_t *array)
{
    //This will need pipeline to properly clear all values
    free(array->p_first_element);
    ioopm_linked_list_clear(array->iterator_list);
    ioopm_linked_list_destroy(array->iterator_list);
    free(array);
}