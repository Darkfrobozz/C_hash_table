#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Common and error handling data types
typedef union elem elem_t;
typedef struct option option_t;
typedef struct iterator ioopm_iterator_t;


// Function types
typedef int(*hashing_func)(elem_t key, void *arg);
typedef short(*compare_func)(elem_t node_key, elem_t key_fetched);


// Applies to a nodes value
typedef bool(*ioopm_pred_value)(elem_t value, void *arg);
//This transform thing is interesting but feels clunky..
typedef void(*ioopm_transform_value)(elem_t *value, void *arg);
//Used to typecast from general structure
typedef elem_t(*type_cast)(void *arg);


// Used for calculating a value across a function
typedef option_t(*ioopm_calc_value)(elem_t value, void **extra);
typedef elem_t(*ioopm_comb_value)(elem_t c_value, elem_t r_value);
typedef (bool)(*custom_assemble)(ioopm_iterator_t *iter, elem_t *memory);


enum pipe_assemblers{transformer, comparer,
                     remover, controller, mover, breaker, custom};


//Linkedlist types

#define MOVE_ON 0
#define REPLACE 1
#define INSERT_PREVIOUS -1
#define ASCII_GARBAGE 33
#define DEL 127
#define ASCII_SIZ 256
#define RIGHT 1
#define LEFT -1
#define mem_limit 10

//ELEM MACROS
#define elem(x) ((elem_t)(x))
#define elem_adress(x) ((elem_t *)(x))
#define adress_to_elem(x) *(elem_adress(x))


#define BYTE(x) ((char *)(x))
#define option(a,b) ((option_t) {.return_value = a, .success = b}) 
#define garbage ((option_t) {0})

union elem {
  //ELEMENTARY
  int i;
  unsigned int u;
  bool b;
  float f;
  size_t siz;
  enum pipe_assemblers assemble;

  //POINTERS

  char *normal_string;
  void *p;
  elem_t *elem_arr;
};




// Used to return the fail or sucess along with the value of an operation. 
struct option {
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
ioopm_always(elem_t value, void *arg);

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

elem_t
ioopm_element_to_list(elem_t list, elem_t item);

void
ioopm_replace(elem_t *value, void *arg);

// printing functions
void
ioopm_print_string(elem_t *key, void *arg);

void
ioopm_print_number(elem_t *value, void *arg);

//Transform funcs
void 
ioopm_increment_int(elem_t *value, void *arg);


//Cleaning
void
ioopm_clean_strings(elem_t *value, void *arg);

//Type casting
elem_t
ioopm_to_int(void *arg);