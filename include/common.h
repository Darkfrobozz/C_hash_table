#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//Common and error handling data types
typedef union elem elem_t;
typedef struct option option_t;

//Function types
typedef int(*hashing_func)(elem_t key, void *arg);
typedef short(*compare_func)(elem_t node_key, elem_t key_fetched);
typedef int(*complex_compare)(elem_t node_key, elem_t key_fetched);


//applies to a nodes value
typedef bool(*ioopm_pred_value)(elem_t value, void *arg);
typedef void(*ioopm_transform_value)(elem_t *value, void *arg);



//Linkedlist types
typedef struct list ioopm_list_t; /// Meta: structure definition goes in C file
typedef struct node node_t;

#define MOVE_ON 0
#define REPLACE 1
#define INSERT_PREVIOUS -1
#define ASCII_GARBAGE 33
#define DEL 127
#define ASCII_SIZ 256
#define RIGHT 1
#define LEFT 0



union elem
{
  /// TODO: update the names of the fields to something better? 
  int i;
  unsigned int u;
  bool b;
  float f;
  unsigned char *string;
  char *normal_string;
  void *p;
  size_t siz;
  /// other choices certainly possible
};

struct node
{
    node_t *previous;
    elem_t value;
    elem_t key;
    node_t *next;
};

struct list
{
    size_t size;
    node_t *first; //dummy node
    node_t *last; // dummy node
    ioopm_transform_value clean_value;
    ioopm_transform_value clean_key;
    ioopm_list_t *iterator_list;
};

/**
 * @brief Used to return the sucess or fail of an operation a value that has nothing to do with
 *
 */
struct option
{
  elem_t return_value;
  short success;
};



short
ioopm_int_compare(elem_t a, elem_t b);

short
ioopm_string_compare(elem_t key_hash, elem_t key_fetch);

/**
 * @brief Modular key
 * 
 * @param key 
 * @param arg Contains the integer that is to be taken mod against
 * @return index of bucket
 */
int
ioopm_int_hash(elem_t key, void *arg);

/**
 * @brief Modular key
 * 
 * @param key 
 * @param arg Contains the integer that is to be taken mod against
 * @return index of bucket
 */
int
ioopm_string_hash(elem_t key, void *arg);


//Basic pred functions
bool 
ioopm_equals_int(elem_t value, void *arg);

bool 
ioopm_equals_adress(elem_t value, void *arg);

bool
ioopm_lesser_than(elem_t value, void *arg);


// combining functions
elem_t 
add_siz(elem_t A, elem_t B);

elem_t
biggest_siz(elem_t c_siz, elem_t f_siz);

// printing functions
void
ioopm_print_string(elem_t *key, void *arg);

void
ioopm_print_number(elem_t *value, void *arg);

//Transform funcs
void 
ioopm_increment_int(elem_t *value, void *arg);


/**
 * @brief Used to clean one value at a time
 * 
 * @param value pointer to value to clean
 * @param arg 
 */
void
ioopm_clean_value_strings(elem_t *value, void *arg);

elem_t
ioopm_element_to_list(elem_t list, elem_t item);