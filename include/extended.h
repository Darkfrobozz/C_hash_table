#pragma once
#include "hash.h"


//Store data types
typedef struct merchandise merch_t;
typedef struct customer customer_t;
typedef struct shopping_cart s_cart_t;
typedef struct stock_value stock_value_t;


//Store structs
struct merchandise
{
    char *name;
    char *desc;
    int price;
    int hash_stock; //a counter, how many there are in stock total (including all shelfs)
    ioopm_list_t *stock_slots;
};

struct stock_value
{
    int amount;
    merch_t *merch;
};

struct shopping_cart
{
    int user_id;
    ioopm_list_t *shopping_list;
};


/**
 * @brief Allocates memory on the heap for a merch struct
 * 
 * @param name 
 * @param desc 
 * @param price 
 * @return merch_t* 
 */
merch_t *
ioopm_merch_create(char *name, char *desc, int price);

/**
 * @brief Allocates memory on the heap for a stock_value struct
 * 
 * @param i_amount
 * @param i_merch
 * @return stock_value_t 
 */
stock_value_t *
ioopm_stock_value_create(int i_amount, merch_t *i_merch);

int
ioopm_merch_hash(elem_t key, void *arg);

short
ioopm_merch_cmp(elem_t key_hash, elem_t key_fetch);

/**
 * @brief Used to transform merch.
 * 
 * @param value - Takes the values to be edited 
 * @param arg Array of changes
 * IN ORDER
 * NEW MERCH NAME
 * NEW MERCH DESCRIPTION
 * NEW MERCH PRICE 
 */
void
ioopm_transform_merch(elem_t *value, void *arg);

int
ioopm_stock_hash(elem_t shelf, void *arg);

short
ioopm_stock_cmp(elem_t shelf_hash, elem_t shelf_fetch);

/**
 * @brief Function used when removing elements from hashtable. Can only be used
 * when merch is allocated on stack.
 * 
 * @param Adresstokey Used to free allocated chunk
 * @param arg Extra arguments
 */
void
ioopm_clean_merch(elem_t *key, void *arg);

void
ioopm_clean_strings(elem_t *key, void *arg);

/**
 * @brief Cleaning up stock
 * 
 * @param value 
 * @param arg 
 */
void
ioopm_clean_stock(elem_t *value, void *arg);


/**
 * @brief Transforming stock frequency!
 * 
 * @param value 
 * @param arg
 * Frequency 
 */
void
ioopm_transform_stock(elem_t *value, void *arg);


