#pragma once
#include "linked_list.h"
#include "iterator.h"
#include "array.h"

//Iterator init macros
#define destroy(x) ioopm_iterator_destroy(x)
#define create_in(x) ioopm_iterator_t *iter = ioopm_list_iterator(x)

//Iterator move macros
#define insert(x,y,z) ioopm_iterator_insert(x,y,z)
#define prepend(x,y,z) ioopm_iterator_prepend(x,y,z)
#define remove(x) ioopm_iterator_remove(x)
#define jump(x,a) ioopm_iterator_jump(x,a)
#define edit(x,y,z) ioopm_iterator_edit(x,y,z)
#define value(x) ioopm_iterator_current_value(x)
#define key(x) ioopm_iterator_current_key(x)

typedef struct node node_t;

struct node
{
    node_t *previous;
    elem_t value;
    elem_t key;
    node_t *next;
};

struct list
{
    size_t chunk_siz;
    size_t size;
    node_t *first; //dummy node
    node_t *last; // dummy node
    ioopm_transform_value clean_value;
    ioopm_transform_value clean_key;
    ioopm_list_t *iterator_list;
};

struct array {
  //sizes
    size_t chunk_siz;
    size_t elements;
  //pointers
    void *p_first_element;
  //if destroyed it needs to update its iterators...
    ioopm_list_t *iterator_list;
  //functions
    ioopm_transform_value clean_value;
    type_cast caster;
  
};

enum updates{destroyed, removed, inserted};

/**
 * @brief This edits a nodes value
 * 
 * @param list 
 * @param edit Function to edit the node with 
 * @param node_edit 
 * @param arg If no edit, add the value to change to here
 * Else add possible arguments for edit function
 * @return New node and success of operation 
 */
void
ioopm_list_edit(ioopm_list_t *list, 
                ioopm_transform_value edit, 
                node_t *node_edit, void *arg);


/// @brief
/// @param iterator_list
/// @param remove_node
/// @return
void
ioopm_list_remove(ioopm_list_t *iterator_list, 
                  node_t *remove_node, int index);


/// @brief
/// @param prev_node
/// @param i_value
/// @param i_key
/// @param list
/// @return 
void
ioopm_list_insert(node_t *prev_node, elem_t i_value, 
                  elem_t i_key, ioopm_list_t *list, 
                  int index);



//Setting up tracking for update system
ioopm_list_t *
bond_iter(ioopm_list_t *iter_list, ioopm_iterator_t *iter);

ioopm_list_t *
list_track_iter(ioopm_list_t *list, ioopm_iterator_t *iter);

ioopm_list_t *
array_track_iter(array_t *array, ioopm_iterator_t *iter);


/// @brief Applies iter destroy, transformation function used when
/// listing iterator outside of a list's iterator list
/// @param value 
/// @param arg 
void
ioopm_iter_apply_destroy(elem_t *value, void *arg);