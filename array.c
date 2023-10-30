#include "include/array.h"
#include "include/nodes.h"
#include "iterator.h"

array_t *
ioopm_array(size_t element_siz, size_t array_siz, type_cast caster)
{
    array_t *new_array = calloc(1, sizeof(array_t));
    void *new_data = calloc(array_siz, element_siz);
    void *last = (char *) new_data + ((array_siz - 1) * element_siz);
    new_array->p_first_element = new_data;
    new_array->p_last_element = last; 
    new_array->chunk_siz = element_siz;
    new_array->elements = array_siz;
    new_array->cast = caster;
    return new_array;
}


void
ioopm_array_cleaners(array_t *arr, ioopm_transform_value clean_value)
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