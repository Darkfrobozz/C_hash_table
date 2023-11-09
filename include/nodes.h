#include "linked_list.h"


typedef struct node node_t;

struct node
{
    node_t *previous;
    elem_t value;
    elem_t key;
    node_t *next;
    bool pre_alloced;
};

struct list
{
    size_t size;
    //Allocate in list
    node_t first; //dummy node
    node_t last; // dummy node

    ioopm_transform_value clean_value;
    ioopm_transform_value clean_key;
    ioopm_list_t *iterator_list;
    //Smart allocation
    node_t *pre_alloc;
    int pre_alloc_index;
    size_t pre_alloc_size;
};


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
option_t
ioopm_edit_node_value(ioopm_list_t *list, 
                      ioopm_transform_value edit, 
                      node_t *node_edit, void *arg);


/// @brief
/// @param iterator_list
/// @param remove_node
/// @return
option_t 
ioopm_remove_node(ioopm_list_t *iterator_list, node_t *remove_node);

/// @brief
/// @param prev_node
/// @param i_value
/// @param i_key
/// @param list
/// @return 
option_t 
ioopm_insert_node(node_t *prev_node, elem_t i_value, elem_t i_key, 
                  ioopm_list_t *list);

