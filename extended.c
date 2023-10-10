#include "extended.h"

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
short
ioopm_merch_cmp(elem_t key_hash, elem_t key_fetch)
{
  merch_t *f_merch = key_fetch.p;
  merch_t *h_merch = key_hash.p;
  return string_compare(h_merch->name, f_merch->name);
}

//Only works for higher bucket values...
int
ioopm_merch_hash(elem_t key_fetched, void *arg)
{
  merch_t *merch_fetched = key_fetched.p;
  return hash_char(merch_fetched->name[0], arg);
}

void
ioopm_clean_merch(elem_t *key, void *arg)
{
  merch_t *merch = key->p;
  free(merch->name);
  free(merch->desc);
  ioopm_hash_table_clear(merch->hash_stock);
  ioopm_hash_table_destroy(merch->hash_stock);
  free(merch);
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