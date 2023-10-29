#include "include/common.h"
#include <stdbool.h>
#include <stdio.h>

static
int
mod(int x, int n)
{
    return (x % n + n) % n;
}

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
  int merch_first_letter = mod(b, ASCII_SIZ);

  if(merch_first_letter > *buckets)
  {
    merch_first_letter = *buckets - 1;
  }

  return merch_first_letter;
}

// String Hashing
short
ioopm_string_compare(elem_t key_hash, elem_t key_fetch)
{
  return string_compare(key_hash.normal_string, key_fetch.normal_string);
}

int 
ioopm_string_hash(elem_t key, void *arg)
{
    return hash_char(key.normal_string[0], arg);
}


//Int hashing
int
ioopm_int_hash(elem_t key, void *arg)
{
    int *mod = arg;
    return key.i % (*mod);
}

short
ioopm_int_compare(elem_t key_hash, elem_t key_fetch)
{
    //move on command
    if(key_hash.i < key_fetch.i)
        return MOVE_ON;

    //replace command
    if(key_hash.i == key_fetch.i)
        return REPLACE;

    //insert prior to this command
    return INSERT_PREVIOUS;
}

//OTHERS
int 
add_int_elements(elem_t a, elem_t b)
{
    return b.i - a.i;
}

//pred
bool 
ioopm_equals_int(elem_t value, void *arg)
{
    int *compare = arg;
    return (value.i == (*compare));
}

//compare void pointers

bool 
ioopm_equals_adress(elem_t value, void *arg)
{
    return (value.p == arg);
}

//Transform function
void 
ioopm_increment_int(elem_t *value, void *arg)
{
    int *inc = arg;
    (*value).i = (*value).i + (*inc);
}

bool
ioopm_lesser_than(elem_t value, void *arg)
{
    int *constant = arg;
    return (value.i < (*constant));
}

elem_t 
add_siz(elem_t A, elem_t B)
{
    elem_t result;
    result.siz = A.siz + B.siz;
    return result;
}

elem_t
biggest_siz(elem_t c_siz, elem_t f_siz)
{
  if(c_siz.siz < f_siz.siz)
    return f_siz;
  return c_siz;
}

//Cleaning func
void
ioopm_clean_strings(elem_t *value, void *arg)
{
    char *string = value->normal_string;
    assert(string);
    free(string);
}