#include "common.h"
#include <stdbool.h>
#include <stdio.h>

// Compare
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

short
ioopm_string_compare(elem_t key_hash, elem_t key_fetch)
{
  unsigned char *key_in_hash = key_hash.string;
  unsigned char *key_fetched = key_fetch.string;
  if(!key_in_hash && !key_fetched)
  {
    return REPLACE;
  }
  if(!key_fetched)
    return INSERT_PREVIOUS;
  if(!key_in_hash)
    return MOVE_ON;
  //while they are equal.
  while (*key_in_hash)
  {
    //"AC" "A"
    if(!(*key_fetched))
      return INSERT_PREVIOUS;
    // "AC" "AB"
    if(*key_in_hash > *key_fetched)
      return INSERT_PREVIOUS;
    //"AB" "AC"
    if(*key_in_hash < *key_fetched)
      return MOVE_ON;
    key_in_hash++;
    key_fetched++;
  }

  // "" ""
  if(*key_in_hash == *key_fetched)
    return REPLACE;
  //ABB ABBA
  return MOVE_ON;
}

int 
ioopm_string_hash(elem_t key, void *arg)
{
    int index = (int) (*(key.string));
    return index - ASCII_GARBAGE;
}

int
ioopm_int_hash(elem_t key, void *arg)
{
    int *mod = arg;
    return key.i % (*mod);
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

void
ioopm_print_string(elem_t *key, void *arg)
{
  unsigned char *string = (*key).string;
  if(string)
    printf("%s: ", string);
}

void
ioopm_print_number(elem_t *value, void *arg)
{
  printf("%d\n", value->i);
}