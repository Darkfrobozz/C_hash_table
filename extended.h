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

stock_value_t
ioopm_stock_value_create(int amount, merch_t *merch);

int
ioopm_merch_hash(elem_t key, void *arg);

short
ioopm_merch_cmp(elem_t key_hash, elem_t key_fetch);

int
ioopm_stock_hash(elem_t shelf, void *arg);

/**
 * @brief Function used when removing elements from hashtable. Can only be used
 * when merch is allocated on stack.
 * 
 * @param Adresstokey Used to free allocated chunk
 * @param arg Extra arguments
 */
void
ioopm_clean_merch(elem_t *key, void *arg);

short
ioopm_stock_cmp(elem_t shelf_hash, elem_t shelf_fetch);

void
ioopm_clean_strings(elem_t *key, void *arg);