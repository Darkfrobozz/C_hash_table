#pragma once
#include "hash.h"


//Store data types
typedef struct merchandise merch_t;
typedef struct customer customer_t;
typedef struct shopping_cart s_cart_t;


//Store structs
struct merchandise
{
    char *name;
    char *desc;
    char *shelf;
    int price;
    ioopm_hash_table_t *hash_stock; //list containing elements, with key being shelf and value being freq
};

struct shopping_cart
{
    int user_id;
    ioopm_list_t *shopping_list;
};

int
ioopm_merch_hash(elem_t key, void *arg);

short
ioopm_merch_cmp(elem_t key_hash, elem_t key_fetch);

int
ioopm_stock_hash(elem_t shelf, void *arg);

short
ioopm_stock_cmp(elem_t shelf_hash, elem_t shelf_fetch);