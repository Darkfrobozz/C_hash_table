#include "extended.h"

short
ioopm_merch_cmp(elem_t key_hash, elem_t key_fetch)
{
  merch_t *f_merch = key_fetch.p;
  merch_t *h_merch = key_hash.p;
  int result = strcmp(h_merch->name, f_merch->name);

  if(!result)
    return REPLACE;
  if(result > 0)
    return INSERT_PREVIOUS;
  
  return MOVE_ON;
}

//Only works for higher bucket values...
int
ioopm_merch_hash(elem_t key_fetched, void *arg)
{
  int *buckets = arg;
  merch_t *merch_fetched = key_fetched.p;
  char merch_first_letter = merch_fetched->name[0];

  return merch_first_letter % (*buckets);
}

