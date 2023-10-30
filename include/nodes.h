#include "linked_list.h"


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
    size_t size;
    node_t *first; //dummy node
    node_t *last; // dummy node
    ioopm_transform_value clean_value;
    ioopm_transform_value clean_key;
    ioopm_list_t *iterator_list;
};

struct array {
  //For example int is chunk size 4
    size_t chunk_siz;
  //For example int[9], chunk amount is 9
    size_t chunk_amount;
    void *p_first_element;
  //if destroyed it needs to update its iterators...
    ioopm_list_t *iterator_list;
    ioopm_transform_value clean_value;
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

ioopm_list_t *
ioopm_iterator_list_create(void);
