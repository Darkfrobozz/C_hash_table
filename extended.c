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


merch_t *
ioopm_merch_create(char *i_name, char *i_desc, int i_price)
{
  merch_t *merch = calloc(1, sizeof(merch_t));
  merch->name = i_name;
  merch->desc = i_desc;
  merch->price = i_price;
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


