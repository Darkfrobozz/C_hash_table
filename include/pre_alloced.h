#pragma once

typedef struct pre_alloc pre_alloc_t;

enum pre_alloc_types{list_alloc, node_alloc};

/**
 * @brief Generates a pre alloc struct
 * 
 * @param siz 
 * @param type 
 * @return pre_alloc_t* 
 */
pre_alloc_t *
ioopm_gen_pre_alloc(size_t siz, enum pre_alloc_types type);

/**
 * @brief This does not take ownership of cleaning the stuff, which means clearing should be done through the actual thing.
 * 
 * @param result 
 * @return true 
 * @return false 
 */
bool
ioopm_destruct_pre_alloc(pre_alloc_t *result);

/**
 * @brief Fetches a free element from pre_alloc and
 * then gives false if pre_alloc has space for inserts.
 * 
 * @param alloc_s 
 * @param type 
 * @return option_t 
 */
option_t
ioopm_pre_alloc_get(pre_alloc_t *alloc_s);

/**
 * @brief Removes and places in dynamic list so that we can reuse it
 * 
 * @param alloc_s 
 * @param index 
 * @return option_t 
 */
option_t
ioopm_remove_alloc_slot(pre_alloc_t *alloc_s, int index);

/**
 * @brief Removes and places in dynamic list so that we can reuse it
 * This uses an adress
 * 
 * @param alloc_s 
 * @param index 
 * @return option_t 
 */
option_t
ioopm_remove_alloc_adress(pre_alloc_t *alloc_s, void *adress);

/**
 * @brief It should be very important to recursively clear
 * the list
 * 
 * @param alloc_s 
 * @return true 
 * @return false 
 */
bool
ioopm_recursive_clear_list_alloc(pre_alloc_t *alloc_s, 
                                 ioopm_transform_value clean_key,
                                 ioopm_transform_value clean_value);
