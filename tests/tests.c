#include <CUnit/Basic.h>
#include "../include/hash.h"
#include "../include/extended.h"
#include "../include/iterator.h"
#include <stdlib.h>
#define arr_siz 100
#define test_array_siz 100
#define no_buckets 17

int 
init_suite(void) 
{
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int 
clean_suite(void) 
{
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

// These are example test functions. You should replace them with
// functions of your own.
void test_list_init()
{
  ioopm_list_t *test_link = ioopm_linked_list_create();
  CU_ASSERT_PTR_NOT_NULL(test_link);
  ioopm_linked_list_destroy(test_link);
}

void test_link_insert()
{
  ioopm_list_t *test_link = ioopm_linked_list_create();
  elem_t send_values[10];
  option_t received_values[10];
  send_values[0].i = 5;
  send_values[1].i = 7;
  send_values[2].i = 5;

  ioopm_linked_list_insert(test_link, 0, send_values[0], (elem_t) NULL);
  received_values[0] = ioopm_linked_list_get(test_link, 0);
  CU_ASSERT_EQUAL(received_values[0].return_value.i, 5);

  ioopm_linked_list_insert(test_link, 1, send_values[1], (elem_t) NULL);
  received_values[1] = ioopm_linked_list_get(test_link, 1);
  CU_ASSERT_EQUAL(received_values[1].return_value.i, 7);

  ioopm_linked_list_insert(test_link, 0, send_values[2], (elem_t) NULL);
  received_values[2] = ioopm_linked_list_get(test_link, 0);
  CU_ASSERT_EQUAL(received_values[2].return_value.i, 5);

  ioopm_linked_list_clear(test_link);
  ioopm_linked_list_destroy(test_link);

}

void test_link_append()
{

  ioopm_list_t *link = ioopm_linked_list_create();
  elem_t send_values[10];
  option_t received_values[10];
  send_values[0].i = 5;
  send_values[1].i = 9;
  send_values[2].i = 15;
  ioopm_linked_list_append(link, send_values[0], (elem_t) NULL);
  received_values[0] = ioopm_linked_list_get(link, 0);
  CU_ASSERT_EQUAL(received_values[0].return_value.i, 5)
  
  ioopm_linked_list_append(link, send_values[1], (elem_t) NULL);
  ioopm_linked_list_append(link, send_values[2], (elem_t) NULL);
  
  received_values[1] = ioopm_linked_list_get(link, 0);
  CU_ASSERT_EQUAL(received_values[1].return_value.i, 9)
  received_values[2] = ioopm_linked_list_get(link, 0);
  CU_ASSERT_EQUAL(received_values[2].return_value.i, 15)
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
}

void test_link_prepend()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  elem_t send_values[test_array_siz];
  for(int i = 0; i < test_array_siz; i++)
  {
    send_values[i].i = i;
    ioopm_linked_list_prepend(link, send_values[i], (elem_t) NULL);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(link, 0).return_value.i, i);
  }
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
}

void test_link_get()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  elem_t send_values[test_array_siz];
  for(int i = 0; i < test_array_siz; i++)
  {
    send_values[i].i = i + 1;
    ioopm_linked_list_append(link, send_values[i], (elem_t) NULL);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(link, i).return_value.i, i + 1);
  }
  
  for(int i = 0; i < test_array_siz; i++)
  {
    int index = rand() % 150;
    option_t received_value = ioopm_linked_list_get(link, index);
    if(received_value.success)
    {
      CU_ASSERT_EQUAL(received_value.return_value.i, index + 1);
      continue;
    }
  }
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
  
}

void test_link_remove()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  elem_t send_values[test_array_siz];
  for(int i = 0; i < test_array_siz; i++)
  {
    send_values[i].i = i;
    ioopm_linked_list_prepend(link, send_values[i], (elem_t) NULL);
    CU_ASSERT_EQUAL(ioopm_linked_list_remove(link, 0).return_value.i, i);
  }
  CU_ASSERT_EQUAL(ioopm_linked_list_size(link), 0);
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);

}

void test_list_contains()
{  
  ioopm_list_t *link = ioopm_linked_list_create();
  elem_t send_values[test_array_siz];
  for(int i = 0; i < test_array_siz; i++)
  {
    send_values[i].i = i;
    ioopm_linked_list_append(link, send_values[i], (elem_t) NULL);
  }
  CU_ASSERT(ioopm_linked_list_contains(link, (elem_t) 5, ioopm_equals_int))
  CU_ASSERT(ioopm_linked_list_contains(link, (elem_t) 8, ioopm_equals_int))
  CU_ASSERT(ioopm_linked_list_contains(link, (elem_t) 10, ioopm_equals_int))
  CU_ASSERT_FALSE(ioopm_linked_list_contains(link, (elem_t) 100, ioopm_equals_int))
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
}

void test_list_all()
{  
  ioopm_list_t *link = ioopm_linked_list_create();
  elem_t send_values[test_array_siz];
  for(int i = 0; i < test_array_siz; i++)
  {
    send_values[i].i = 5;
    ioopm_linked_list_append(link, send_values[i], (elem_t) NULL);
  }

  int value = 5;
  CU_ASSERT(ioopm_linked_list_all(link, ioopm_equals_int, &value));
  value = 12;
  CU_ASSERT_FALSE(ioopm_linked_list_all(link, ioopm_equals_int, &value));
  send_values[0].i = 12;
  CU_ASSERT_FALSE(ioopm_linked_list_all(link, ioopm_equals_int, &value));

  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);

}

void test_increment()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  elem_t send_values[test_array_siz];
  for(int i = 0; i < test_array_siz; i++)
  {
    send_values[i].i = 5;
    ioopm_linked_list_append(link, send_values[i], (elem_t) NULL);
  }
  int inc = 1;
  ioopm_linked_list_apply_to_all(link, ioopm_increment_int, &inc);
  int value = 6;
  CU_ASSERT(ioopm_linked_list_all(link, ioopm_equals_int, &value));
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
}

void test_create_iterator()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  ioopm_iterator_t *iter1 = ioopm_list_iterator(link);
  ioopm_iterator_t *iter2 = ioopm_list_iterator(link);
  ioopm_iterator_t *iter3 = ioopm_list_iterator(link);
  ioopm_iterator_t *iter4 = ioopm_list_iterator(link);
  ioopm_linked_list_append(link, (elem_t) NULL, (elem_t) NULL);
  ioopm_linked_list_append(link, (elem_t) NULL, (elem_t) NULL);
  ioopm_linked_list_append(link, (elem_t) NULL, (elem_t) NULL);
  ioopm_iterator_destroy(iter1);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(link->iterator_list), 3);
  ioopm_iterator_destroy(iter2);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(link->iterator_list), 2);
  ioopm_iterator_has_next(iter3);
  ioopm_iterator_has_next(iter4);
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
  
}

void test_iterator_has_next()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  ioopm_iterator_t *iter1 = ioopm_list_iterator(link);
  ioopm_iterator_t *iter2 = ioopm_list_iterator(link);
  ioopm_iterator_t *iter3 = ioopm_list_iterator(link);
  CU_ASSERT_FALSE(ioopm_iterator_has_next(iter3));

  elem_t send_values[3];
  elem_t received_values[3];
  send_values[0].i = 5;
  send_values[1].i = 9;
  send_values[2].i = 15;
  ioopm_linked_list_append(link, send_values[0], (elem_t) NULL);
  ioopm_linked_list_append(link, send_values[1], (elem_t) NULL);
  ioopm_linked_list_append(link, send_values[2], (elem_t) NULL);
  ioopm_iterator_next(iter2);
  CU_ASSERT(ioopm_iterator_has_next(iter3));
  ioopm_iterator_next(iter3);
  ioopm_iterator_next(iter3);
  CU_ASSERT_FALSE(ioopm_iterator_has_next(iter3));
  received_values[0] = ioopm_iterator_current_value(iter1).return_value;
  received_values[1] = ioopm_iterator_current_value(iter2).return_value;
  received_values[2] = ioopm_iterator_current_value(iter3).return_value;

  CU_ASSERT_EQUAL(received_values[0].i, 5);
  CU_ASSERT_EQUAL(received_values[1].i, 9);
  CU_ASSERT_EQUAL(received_values[2].i, 15);
  ioopm_linked_list_remove(link, 0);
  received_values[0] = ioopm_iterator_current_value(iter1).return_value;
  CU_ASSERT_EQUAL(received_values[0].i, 9);
  ioopm_linked_list_remove(link, 1);
  received_values[2] = ioopm_iterator_current_value(iter3).return_value;
  CU_ASSERT_EQUAL(received_values[2].i, 9);

  ioopm_iterator_destroy(iter1);
  ioopm_iterator_destroy(iter2);
  ioopm_iterator_destroy(iter3);
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);

}

void test_remove_iterator()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  ioopm_iterator_t *iter_a = ioopm_list_iterator(link);

  option_t remove_empty = ioopm_iterator_remove(iter_a);
  CU_ASSERT_FALSE(remove_empty.success);
  
  elem_t send_values[3];
  send_values[0].i = 3;
  send_values[1].i = 12;
  send_values[2].i = 17;

  ioopm_linked_list_append(link, send_values[0], (elem_t) NULL);
  ioopm_linked_list_append(link, send_values[1], (elem_t) NULL);
  ioopm_linked_list_append(link, send_values[2], (elem_t) NULL);

  option_t remove_first = ioopm_iterator_remove(iter_a);
  CU_ASSERT(remove_first.success);
  CU_ASSERT_EQUAL(remove_first.return_value.i, send_values[0].i);
  
  ioopm_iterator_next(iter_a);
  option_t remove_third = ioopm_iterator_remove(iter_a);
  CU_ASSERT(remove_third.success);
  CU_ASSERT_EQUAL(remove_third.return_value.i, send_values[2].i);
  CU_ASSERT_EQUAL(ioopm_iterator_current_value(iter_a).return_value.i, send_values[1].i);
  ioopm_iterator_destroy(iter_a);
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
}

void double_iterator()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  ioopm_iterator_t *iter_a = ioopm_list_iterator(link);
  ioopm_iterator_t *iter_b = ioopm_list_iterator(link);

  elem_t send_values[3];

  send_values[0].i = 3;
  send_values[1].i = 9;
  send_values[2].i = 15;
  ioopm_iterator_insert(iter_a, send_values[0], (elem_t) NULL, RIGHT);
  ioopm_iterator_insert(iter_a, send_values[1], (elem_t) NULL, LEFT);

  ioopm_iterator_remove(iter_b);

  CU_ASSERT_EQUAL(ioopm_iterator_current_value(iter_a).return_value.i, send_values[1].i);

  ioopm_iterator_insert(iter_b, send_values[2], (elem_t) NULL, RIGHT);
  ioopm_iterator_next(iter_a);

  CU_ASSERT_EQUAL(ioopm_iterator_current_value(iter_a).return_value.i, send_values[2].i);
  CU_ASSERT_EQUAL(ioopm_iterator_current_value(iter_b).return_value.i, send_values[2].i);

  ioopm_iterator_remove(iter_a);
  CU_ASSERT_EQUAL(ioopm_iterator_current_value(iter_b).return_value.i, send_values[1].i);

  ioopm_iterator_destroy(iter_a);
  ioopm_iterator_destroy(iter_b);
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
}


void 
test_create_remove(void) 
{
    ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
    ioopm_int_compare, no_buckets);
    ioopm_hash_table_destroy(hash);    
}


//ASSIGNMENTSTEP1
void 
test_lookup(void)
{
    ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
    ioopm_int_compare, no_buckets);
    elem_t sendvalues[3];
    sendvalues[0].i = 844;
    option_t result = ioopm_hash_table_lookup(hash, sendvalues[0]);
    CU_ASSERT_FALSE(result.success);
    ioopm_hash_table_clear(hash);
    ioopm_hash_table_destroy(hash);
}

void
test_insert(void)
{
    ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
    ioopm_int_compare, no_buckets);
    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];

    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand();
      randomkeys[i].i = rand();
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }

    for(int i = 0; i < arr_siz; i++)
    {
      int index = rand() % arr_siz;

      CU_ASSERT_EQUAL(ioopm_hash_table_lookup(hash, 
      randomkeys[index]).return_value.i, sendvalues[index].i);
    }
    ioopm_hash_table_clear(hash);
    ioopm_hash_table_destroy(hash); 
}

void
test_insert_overwrite(void)
{
    ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
    ioopm_int_compare, no_buckets);
    elem_t sendvalues[3];
    elem_t randomkeys[3];

    sendvalues[0].i = 76;
    sendvalues[1].i = 760;
    sendvalues[2].i = 7600;
    randomkeys[0].i = 1;
    randomkeys[1].i = 1 + no_buckets;
    randomkeys[2].i = 1 + no_buckets * 2;

    ioopm_hash_table_insert(hash, randomkeys[0], sendvalues[0]);
    ioopm_hash_table_insert(hash, randomkeys[1], sendvalues[1]);
    ioopm_hash_table_insert(hash, randomkeys[2], sendvalues[2]);

    ioopm_hash_table_insert(hash, randomkeys[0], sendvalues[2]);
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(hash, 
                    randomkeys[0]).return_value.i, sendvalues[2].i);
    ioopm_hash_table_insert(hash, randomkeys[1], sendvalues[0]);
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(hash, 
                    randomkeys[1]).return_value.i, sendvalues[0].i);
    ioopm_hash_table_insert(hash, randomkeys[2], sendvalues[1]);
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(hash, 
                    randomkeys[2]).return_value.i, sendvalues[1].i);

    ioopm_hash_table_clear(hash);
    ioopm_hash_table_destroy(hash); 
}


//ASSIGNMENTSTEP4 and 5
void
test_remove(void)
{
    ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
    ioopm_int_compare, no_buckets);
    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];
    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand();
      randomkeys[i].i = rand() % 100 + 100 * i;
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }

    for(int i = 0; i < arr_siz; i++)
    {
      int index = rand() % arr_siz;
      option_t result = ioopm_hash_table_remove(hash, randomkeys[index]);
      if(result.success)
      CU_ASSERT_EQUAL(result.return_value.i, sendvalues[index].i);

    }

    ioopm_hash_table_clear(hash);
    ioopm_hash_table_destroy(hash); 
} 

//ASSIGNMENT1STEP6
void
test_size()
{
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
  ioopm_int_compare, no_buckets);
  elem_t value;
  elem_t key;
  value.i = 0;
  CU_ASSERT_EQUAL(ioopm_hash_table_size(hash), 0);

  for(int i = 0; i < arr_siz; i++)
  {
    key.i = i;
    ioopm_hash_table_insert(hash, key, value);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(hash), i + 1);
  }
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash); 
  

  
}

void
test_empty()
{
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
  ioopm_int_compare, no_buckets);
  CU_ASSERT(ioopm_hash_table_is_empty(hash));
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash); 
  
}

void
test_clear()
{ 
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
  ioopm_int_compare, no_buckets);
  elem_t value;
  elem_t key;
  key.i = 1;
  value.i = 0;
  ioopm_hash_table_insert(hash, key, value);
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(hash));
  ioopm_hash_table_clear(hash);
  CU_ASSERT(ioopm_hash_table_is_empty(hash));
  ioopm_hash_table_destroy(hash); 
}

//ASSIGNMENTSTEP7

void
test_key_list()
{
    ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
    ioopm_int_compare, no_buckets);
    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];
    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand();
      randomkeys[i].i = 100 * i + rand() % 100;
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }
    ioopm_list_t *list = ioopm_hash_table_keys(hash);
    //return a list where the desired thing (aka keys or values) is values.
    for(int i = 0; i < arr_siz; i++)
    {
      CU_ASSERT(ioopm_linked_list_contains(list, randomkeys[i], ioopm_equals_int));
    }
    ioopm_linked_list_clear(list);
    ioopm_linked_list_destroy(list);
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash); 
}

void
test_value_list()
{
    ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
    ioopm_int_compare, no_buckets);
    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];
    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand();
      randomkeys[i].i = 100 * i + rand() % 100;
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }
    ioopm_list_t *list = ioopm_hash_table_values(hash);
    //return a list where the desired thing (aka keys or values) is values.
    for(int i = 0; i < arr_siz; i++)
    {
      CU_ASSERT(ioopm_linked_list_contains(list, sendvalues[i], ioopm_equals_int));
    }
  ioopm_linked_list_clear(list);
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
  ioopm_linked_list_destroy(list);
}

//ASSIGNMENTSTEP8
void
test_has_key()
{
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
  ioopm_int_compare, no_buckets);

  elem_t key;
  key.i = 5;
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(hash, key));

    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];
    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand();
      randomkeys[i].i = 100 * i + rand() % 100;
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }
    for(int i = 0; i < arr_siz; i++)
    {
      CU_ASSERT(ioopm_hash_table_has_key(hash, randomkeys[i]));
    }
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
}

void
test_has_value()
{
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
  ioopm_int_compare, no_buckets);

  elem_t value;
  value.i = 9;
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(hash, value));

    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];
    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand();
      randomkeys[i].i = 100 * i + rand() % 100;
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }
    for(int i = 0; i < arr_siz; i++)
    {
      CU_ASSERT(ioopm_hash_table_has_value(hash, sendvalues[i],
                                           ioopm_equals_int));
    }
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
}

//ASSIGNMENTSTEP9
void
test_any()
{
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
  ioopm_int_compare, no_buckets);

  elem_t value;
  value.i = 9;
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(hash, value));

    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];
    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand();
      randomkeys[i].i = 100 * i + rand() % 100;
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }
    for(int i = 0; i < arr_siz; i++)
    {
      CU_ASSERT(ioopm_hash_table_any(hash, ioopm_equals_int,
                                     sendvalues + i));
    }
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
}


void 
test_all()
{
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
  ioopm_int_compare, no_buckets);

  elem_t value;
  value.i = 9;
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(hash, value));

    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];
    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand() % 100;
      randomkeys[i].i = 100 * i + rand() % 100;
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }
    int storage = 100;
    CU_ASSERT(ioopm_hash_table_all(hash, ioopm_lesser_than,
                                   &storage));
    storage = 0;
    CU_ASSERT_FALSE(ioopm_hash_table_all(hash, ioopm_lesser_than,
                                   &storage));

  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
}

//ASSIGNMENTSTEP10
void
test_apply_all()
{
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
  ioopm_int_compare, no_buckets);

  elem_t value;
  value.i = 9;
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(hash, value));

    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];
    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand();
      randomkeys[i].i = 100 * i + rand() % 100;
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }
    
    int inc = 1;
    ioopm_hash_table_apply_to_all(hash, ioopm_increment_int, &inc);
    
    for(int i = 0; i < arr_siz; i++)
    {
      elem_t value;
      value.i = sendvalues[i].i + 1;
      CU_ASSERT(ioopm_hash_table_has_value(hash, value,
                                           ioopm_equals_int));
    }

    inc = 10;
    ioopm_hash_table_apply_to_all(hash, ioopm_increment_int, &inc);
    
    for(int i = 0; i < arr_siz; i++)
    {
      elem_t value;
      value.i = sendvalues[i].i + 11;
      CU_ASSERT(ioopm_hash_table_has_value(hash, value,
                                           ioopm_equals_int));
    }

  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
}
void
test_hash_stats(void)
{  
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
  ioopm_int_compare, no_buckets);
  elem_t sendvalues[arr_siz];
  for(int i = 0; i < arr_siz; i++)
  {
    sendvalues[i].i = i;
    int index = i % 2 + no_buckets * i;
    elem_t index_elem;
    index_elem.i = index;
    ioopm_hash_table_insert(hash, index_elem, sendvalues[i]);
  }

  ioopm_evaluate_hash(hash);
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
}
void
test_resize(void)
{

}
void
test_merch_hashtable(void)
{
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_merch_hash,
  ioopm_merch_cmp, no_buckets);

  merch_t m_1 = {.name = "KO", .desc = "rund", .price = 15, .hash_stock = 0, NULL};
  elem_t em_1;
  em_1.p = &m_1;
  merch_t m_2 = {.name = "OXE", .desc = "kantig", .price = 14, .hash_stock = 0, NULL};
  elem_t em_2;
  em_2.p = &m_2;
  merch_t m_3 = {.name = "OXI", .desc = "oval", .price = 13, .hash_stock = 0, NULL};
  elem_t em_3;
  em_3.p = &m_3;
  merch_t m_4 = {.name = "KOSSA", .desc = "romb", .price = 12, .hash_stock = 0, NULL};
  elem_t em_4;
  em_4.p = &m_4;
  
  merch_t merch[] = {m_1, m_2, m_3, m_4};
  elem_t merchpointer[] = {em_1, em_2, em_3, em_4};
  
  for(int i = 0; i < 4; i++)
  {
    ioopm_hash_table_insert(hash, (elem_t) merch[i].name, merchpointer[i]);
  }

  for(int i = 0; i < 4; i++)
  {
    option_t value = ioopm_hash_table_lookup(hash, (elem_t) merch[i].name);
    CU_ASSERT(value.success);
  }

  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
}

void
test_stock_hash(void)
{
  ioopm_hash_table_t *merch_table = ioopm_hash_table_create(ioopm_merch_hash,
  ioopm_merch_cmp, no_buckets);

  ioopm_hash_table_t *stock_table = ioopm_hash_table_create(ioopm_stock_hash,
  ioopm_stock_cmp, no_buckets);

  ioopm_hash_add_cleaner(merch_table, NULL, ioopm_clean_merch);
  ioopm_hash_add_cleaner(stock_table, ioopm_clean_strings, NULL);

  merch_t *arr[25];
  stock_value_t stockvals[25];

  //filling merch table with merch A, B, C, D...
  //creating a stock value for them
  for(int index = 0; index < 25; index++)
  {
    char name[] = {65 + index, 0};
    char *string = strdup(name);
    
    char shelfarr[] = {65 + index, index % 10 + 48, index % 10 + 48};
    char *shelf = strdup(shelfarr);

    arr[index] = ioopm_merch_create(string, string, index); 

    stockvals[index] = ioopm_stock_value_create(index, arr[index]);

    //Trying to refill at a different merch or already existing stock - needs its own logic
    ioopm_hash_table_insert(stock_table, (elem_t) shelf, (elem_t) (void *)(stockvals + index));

    ioopm_linked_list_append(arr[index]->stock_slots, (elem_t) shelf, 
                            (elem_t) (void *)(stockvals + index)); 
    
    elem_t m_hashed;
    m_hashed.p = arr[index];
    ioopm_hash_table_insert(merch_table, (elem_t) arr[index]->name, m_hashed);
  }

  //Trying to get the merch through a   
  for(int index = 0; index < 25; index++)
  {
    CU_ASSERT(ioopm_hash_table_lookup(merch_table, (elem_t) arr[index]->name).success);

    char shelfarr[] = {65 + index, index % 10 + 48, index % 10 + 48};
    char *shelf = strdup(shelfarr);

    CU_ASSERT(ioopm_hash_table_has_key(stock_table, (elem_t) shelf));

    free(shelf);
  }

  ioopm_hash_table_clear(stock_table);
  ioopm_hash_table_destroy(stock_table);

  ioopm_hash_table_clear(merch_table);
  ioopm_hash_table_destroy(merch_table);
}


void 
test_relocate_node(void)
{
   ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_int_hash,
   ioopm_int_compare, no_buckets);
  
    elem_t sendvalues[arr_siz];
    elem_t randomkeys[arr_siz];
    for(int i = 0; i < arr_siz; i++)
    {
      sendvalues[i].i = rand() % 100;
      randomkeys[i].i = 100 * i;
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }
  elem_t comparer = ioopm_hash_table_lookup(hash, (elem_t) 100).return_value; 
  ioopm_rehash(hash, (elem_t) 100, (elem_t) 150);
  CU_ASSERT_EQUAL(comparer.i, 
                  ioopm_hash_table_lookup(hash, (elem_t) 150).return_value.i);
  ioopm_rehash(hash, (elem_t) 150, (elem_t) 173);
  CU_ASSERT_EQUAL(comparer.i, 
                  ioopm_hash_table_lookup(hash, (elem_t) 173).return_value.i); 
  CU_ASSERT_FALSE(ioopm_rehash(hash, (elem_t) 100, (elem_t) 173).success);

  elem_t value;
  value.i = 100;
  elem_t key;
  key.i = 173;
  void *bundled_data[] = {&value, &key};
  ioopm_hash_edit(hash, NULL, key, bundled_data);
  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(hash, key).return_value.i, 100); 
  
  CU_ASSERT_FALSE(ioopm_hash_edit(hash, NULL, value, bundled_data).success); 
  
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
}

int 
main(int argc, char *argv[]) 
{

  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite list_suite = CU_add_suite("Testing linked list", init_suite, clean_suite);
  CU_pSuite hash_suite = CU_add_suite("My awesome hash suite", init_suite, clean_suite);
  if (hash_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
    (CU_add_test(list_suite, "Create and destroy linked lists", test_list_init) == NULL) 
    || (CU_add_test(list_suite, "Linked list insertion", test_link_insert) == NULL) 
    || (CU_add_test(list_suite, "Linked list removal", test_link_remove) == NULL) 
    || (CU_add_test(list_suite, "List contains test", test_list_contains) == NULL) 
    || (CU_add_test(list_suite, "List true for all", test_list_all) == NULL) 
    || (CU_add_test(list_suite, "List incrementing", test_increment) == NULL) 
    || (CU_add_test(list_suite, "List get function", test_link_get) == NULL) 
    || (CU_add_test(list_suite, "List prepend function", test_link_prepend) == NULL) 
    || (CU_add_test(list_suite, "Create iterator over list", test_create_iterator) == NULL) 
    || (CU_add_test(list_suite, "Iter move in list", test_iterator_has_next) == NULL) 
    || (CU_add_test(list_suite, "Remove element with iterator", test_remove_iterator) == NULL) 
    || (CU_add_test(list_suite, "Insert and remove with iterator", double_iterator) == NULL) 
    || (CU_add_test(hash_suite, "create test", test_create_remove) == NULL) 
    || (CU_add_test(hash_suite, "Lookup test", test_lookup) == NULL) 
    || (CU_add_test(hash_suite, "insert test", test_insert) == NULL)
    || (CU_add_test(hash_suite, "insert test overwrite", test_insert_overwrite) == NULL)
    || (CU_add_test(hash_suite, "remove test", test_remove) == NULL)
    || (CU_add_test(hash_suite, "size check test", test_size) == NULL)
    || (CU_add_test(hash_suite, "Check if empty test", test_empty) == NULL)
    || (CU_add_test(hash_suite, "Clearing test", test_clear) == NULL)
    || (CU_add_test(hash_suite, "Getting keys", test_key_list) == NULL)
    || (CU_add_test(hash_suite, "Getting values", test_value_list) == NULL)
    || (CU_add_test(hash_suite, "Has key", test_has_key) == NULL)
    || (CU_add_test(hash_suite, "Has value", test_has_value) == NULL)
    || (CU_add_test(hash_suite, "Any on values", test_any) == NULL)
    || (CU_add_test(hash_suite, "True for all", test_all) == NULL)
    || (CU_add_test(hash_suite, "Apply to all", test_apply_all) == NULL)
    || (CU_add_test(hash_suite, "Hashing merch", test_merch_hashtable) == NULL)
    || (CU_add_test(hash_suite, "Stock hash, adding shelves to merch", test_stock_hash) == NULL)
    || (CU_add_test(hash_suite, "Hash Stat", test_hash_stats) == NULL)
    || (CU_add_test(hash_suite, "Rehash and edit", test_relocate_node) == NULL)
    || 0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // This is where the tests are actually run!
  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);
  if(argc < 2)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  else
  {
    if(!strcmp("all", argv[1]))
    {
      CU_basic_run_tests();
    }
    else
    {
      for(int i = 1; i < argc; i++)
      {
        if(!strcmp("list", argv[i]))
        {
          CU_basic_run_suite(list_suite);
          continue;
        }
        if(!strcmp("hash", argv[i]))
          CU_basic_run_suite(hash_suite);

      }
    }
  }
  

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
} 