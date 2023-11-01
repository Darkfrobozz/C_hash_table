#include "include/array.h"
#include "include/nodes.h"
#include "iterator.h"

array_t *
ioopm_array_create(size_t element_siz, size_t array_siz)
{
    array_t *new_array = calloc(1, sizeof(array_t));
    void *new_data = calloc(array_siz, element_siz);

    ioopm_array_set(new_array, array_siz, element_siz, new_data);
    
    return new_array;
}

void
ioopm_array_set(array_t *arr, size_t array_siz, size_t elem_siz, elem_t *first)
{
    arr->p_first_element = first;
    arr->chunk_siz = elem_siz;
    arr->elements = array_siz;
}

void
ioopm_array_cleaners(array_t *arr, ioopm_transform_value clean_value)
{
    arr->clean_value = clean_value;
}

ioopm_list_t *
array_track_iter(array_t *array, ioopm_iterator_t *iter)
{
    array->iterator_list = bond_iter(array->iterator_list, iter);
    return array->iterator_list;
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