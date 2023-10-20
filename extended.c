#include "include/extended.h"

static
short
string_compare(char *hash_string, char *fetched_string)
{
  int result = strcmp(hash_string, fetched_string);

  if(!result)
    return REPLACE;
  if(result > 0)
    return INSERT_PREVIOUS;
  
  return MOVE_ON;
}

static
int
hash_char(char b, void *arg)
{ 
  int *buckets = arg;
  int merch_first_letter = b % ASCII_SIZ;

  if(merch_first_letter > *buckets)
  {
    merch_first_letter = *buckets - 1;
  }

  return merch_first_letter;
}



// FOR MERCH HASH TABLE
// Fix normal string WHAT IS THIS

short
ioopm_merch_cmp(elem_t key_hash, elem_t key_fetch)
{
  return string_compare(key_hash.normal_string, key_fetch.normal_string);
}

//Only works for higher bucket values...
int
ioopm_merch_hash(elem_t key_fetched, void *arg)
{
  return hash_char(key_fetched.normal_string[0], arg);
}


merch_t *
ioopm_merch_create(char *i_name, char *i_desc, int i_price)
{
  merch_t *merch = calloc(1, sizeof(merch_t));
  merch->name = i_name;
  merch->desc = i_desc;
  merch->price = i_price;
  merch->hash_stock = 0;
  merch->stock_slots = ioopm_linked_list_create();
  return merch;
}

void
ioopm_clean_merch(elem_t *key, void *arg)
{
  merch_t *merch = key->p;
  assert(merch->name);
  assert(merch->desc);
  if(merch->name != merch->desc)
  {
    free(merch->name);
  }
  free(merch->desc);
  ioopm_linked_list_clear(merch->stock_slots);
  ioopm_linked_list_destroy(merch->stock_slots);
  free(merch);
}

void
ioopm_transform_merch(elem_t *value, void *arg)
{
  char **changes = arg;
  merch_t *current_m = value->p;
  if(changes[0] && changes[0] )
  {
    current_m->name = changes[0];
  }
  
}

//FOR STOCK HASH_TABLE

short
ioopm_stock_cmp(elem_t shelf_hash, elem_t shelf_fetch)
{
  return string_compare(shelf_hash.normal_string, shelf_fetch.normal_string);
}

int
ioopm_stock_hash(elem_t shelf, void *arg)
{
  return hash_char(shelf.normal_string[0], arg);
}

void
ioopm_clean_strings(elem_t *value, void *arg)
{
    char *string = value->normal_string;
    assert(string);
    free(string);
}

void
ioopm_clean_stock(elem_t *value, void *arg)
{
    stock_value_t *val = value->p;
    free(val);
}

stock_value_t
ioopm_stock_value_create(int i_amount, merch_t *i_merch)
{
  return (stock_value_t) { .amount = i_amount, .merch = i_merch};
}