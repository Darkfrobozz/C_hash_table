#include "include/common.h"
#include <stdbool.h>
#include <stdio.h>
#define END_CHAR_INTERVAL 123
#define START_CHAR_INTERVAL 64
#define INTERVAL_LENGTH 59

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

//This needs to change
static
int
hash_char(char b, int size)
{ 
  //Care less about early merch letters
  //This is size
  //Less than 65 can be grouped
  //Focus on 64 - 122
  //IS THIS NOT GREATER RARE CHAR MINUS LESSER RARE CHAR
  //More than 122 Can be grouped
  int merch_first_letter = mod(b, ASCII_SIZ);

  if(merch_first_letter <= START_CHAR_INTERVAL)
    merch_first_letter = 0;

  if(merch_first_letter >= END_CHAR_INTERVAL)
    merch_first_letter = END_CHAR_INTERVAL - START_CHAR_INTERVAL;
  
  //Making sure it works for low size
  if(merch_first_letter > size)
    merch_first_letter = size - 1;

  return mod(merch_first_letter, END_CHAR_INTERVAL - START_CHAR_INTERVAL);
}

short
ioopm_string_compare(elem_t key_hash, elem_t key_fetch)
{
  return string_compare(key_hash.normal_string, key_fetch.normal_string);
}


// String Hashing
//This needs to be developed
/**
 * @brief The following hash function keeps the string ordered by subjecting reserving a segment for each part of a string
 * Forexample a large hash map on ABC, stores A on slot 59 of a segment B on slot starting at slot 118 and this is stored on a segment C on 10443
 * Valid hash sizes are:
 * 60
 * 3482
 * 205380
 * 12 117 362
 * 714 924 299
 * 
 * @param key String to be hashed 
 * @param arg 
 * @return HASHKEY 
 */
int 
ioopm_string_hash(elem_t key, void *arg)
{
  int *size = arg;
  //We should char by multiples of END_CHAR_INTERVAL
  int char_index = 1;
  int product = 1;
  int str_length = strlen(key.normal_string);
  int hash_key = 0;

  for(;product * INTERVAL_LENGTH < *size && char_index < str_length; char_index++)
  {
    hash_key += hash_char(key.normal_string[char_index], *size) * product;
    product *= INTERVAL_LENGTH;
  } 

  return hash_key;
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