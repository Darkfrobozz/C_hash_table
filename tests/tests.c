#include <CUnit/Basic.h>
#include "../include/hash.h"
#include "../include/iterator.h"
#include "../include/array.h"
#include "../include/pipeline.h"
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

  ioopm_list_append(test_link, send_values[0], (elem_t) NULL);
  received_values[0] = ioopm_linked_list_get(test_link, 0);
  CU_ASSERT_EQUAL(received_values[0].return_value.i, 5);

  ioopm_list_append(test_link, send_values[1], (elem_t) NULL);
  received_values[1] = ioopm_linked_list_get(test_link, 1);
  CU_ASSERT_EQUAL(received_values[1].return_value.i, 7);

  ioopm_linked_list_insert(test_link, 0, send_values[2], (elem_t) NULL);
  received_values[2] = ioopm_linked_list_get(test_link, 1);
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
  ioopm_list_append(link, send_values[0], (elem_t) NULL);
  received_values[0] = ioopm_linked_list_get(link, 0);
  CU_ASSERT_EQUAL(received_values[0].return_value.i, 5)
  
  ioopm_list_append(link, send_values[1], (elem_t) NULL);
  ioopm_list_append(link, send_values[2], (elem_t) NULL);
  
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
    ioopm_list_prepend(link, send_values[i], (elem_t) NULL);
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
    ioopm_list_append(link, send_values[i], (elem_t) NULL);
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

void test_link_remove_at()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  elem_t send_values[test_array_siz];
  for(int i = 0; i < test_array_siz; i++)
  {
    send_values[i].i = i;
    ioopm_list_prepend(link, send_values[i], (elem_t) NULL);
    CU_ASSERT_EQUAL(ioopm_linked_list_remove_at(link, 0).return_value.i, i);
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
    ioopm_list_append(link, send_values[i], (elem_t) NULL);
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
    ioopm_list_append(link, send_values[i], (elem_t) NULL);
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
    ioopm_list_append(link, send_values[i], (elem_t) NULL);
  }
  int inc = 1;
  ioopm_list_edit_values(link, ioopm_increment_int, &inc);
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
  ioopm_list_append(link, (elem_t) NULL, (elem_t) NULL);
  ioopm_list_append(link, (elem_t) NULL, (elem_t) NULL);
  ioopm_list_append(link, (elem_t) NULL, (elem_t) NULL);
  ioopm_iterator_destroy(iter1);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(ioopm_get_iterator(link)), 3);
  ioopm_iterator_destroy(iter2);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(ioopm_get_iterator(link)), 2);
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
  ioopm_list_append(link, send_values[0], (elem_t) NULL);
  ioopm_list_append(link, send_values[1], (elem_t) NULL);
  ioopm_list_append(link, send_values[2], (elem_t) NULL);
  ioopm_iterator_next(iter2);
  CU_ASSERT(ioopm_iterator_has_next(iter3));
  ioopm_iterator_next(iter3);
  ioopm_iterator_next(iter3);
  CU_ASSERT_FALSE(ioopm_iterator_has_next(iter3));
  received_values[0] = ioopm_iterator_value_at(iter1).return_value;
  received_values[1] = ioopm_iterator_value_at(iter2).return_value;
  received_values[2] = ioopm_iterator_value_at(iter3).return_value;

  CU_ASSERT_EQUAL(received_values[0].i, 5);
  CU_ASSERT_EQUAL(received_values[1].i, 9);
  CU_ASSERT_EQUAL(received_values[2].i, 15);
  ioopm_linked_list_remove_at(link, 0);
  received_values[0] = ioopm_iterator_value_at(iter1).return_value;
  CU_ASSERT_EQUAL(received_values[0].i, 9);
  ioopm_linked_list_remove_at(link, 1);
  received_values[2] = ioopm_iterator_value_at(iter3).return_value;
  CU_ASSERT_EQUAL(received_values[2].i, 9);

  ioopm_iterator_destroy(iter1);
  ioopm_iterator_destroy(iter2);
  ioopm_iterator_destroy(iter3);
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);

}

void test_remove_at_iterator()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  ioopm_iterator_t *iter_a = ioopm_list_iterator(link);

  option_t remove_at_empty = ioopm_iterator_value_at(iter_a);
  CU_ASSERT_FALSE(ioopm_iterator_remove_at(iter_a));
  CU_ASSERT_FALSE(remove_at_empty.success);
  
  elem_t send_values[3];
  send_values[0].i = 3;
  send_values[1].i = 12;
  send_values[2].i = 17;

  ioopm_list_append(link, send_values[0], (elem_t) NULL);
  ioopm_list_append(link, send_values[1], (elem_t) NULL);
  ioopm_list_append(link, send_values[2], (elem_t) NULL);
  option_t remove_at_first = ioopm_iterator_value_at(iter_a);
  ioopm_iterator_remove_at(iter_a);
  CU_ASSERT(remove_at_first.success);
  CU_ASSERT_EQUAL(remove_at_first.return_value.i, send_values[0].i);
  
  ioopm_iterator_next(iter_a);
  option_t remove_at_third = ioopm_iterator_value_at(iter_a);
  ioopm_iterator_remove_at(iter_a);
  CU_ASSERT(remove_at_third.success);
  CU_ASSERT_EQUAL(remove_at_third.return_value.i, send_values[2].i);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter_a).return_value.i, send_values[1].i);
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
  ioopm_iterator_insert(iter_a, send_values[0], (elem_t) NULL);
  ioopm_iterator_prepend(iter_a, send_values[1], (elem_t) NULL);

  ioopm_iterator_remove_at(iter_b);

  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter_a).return_value.i, send_values[1].i);

  ioopm_iterator_insert(iter_b, send_values[2], (elem_t) NULL);
  ioopm_iterator_next(iter_a);

  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter_a).return_value.i, send_values[2].i);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter_b).return_value.i, send_values[2].i);

  ioopm_iterator_remove_at(iter_a);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter_b).return_value.i, send_values[1].i);

  ioopm_iterator_destroy(iter_a);
  ioopm_iterator_destroy(iter_b);
  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
}

void test_clearing_iterators()
{
  ioopm_list_t *link = ioopm_linked_list_create();
  ioopm_iterator_t *iter_a = ioopm_list_iterator(link);
  ioopm_iterator_t *iter_b = ioopm_list_iterator(link);
  ioopm_list_iterator(link);
  ioopm_list_iterator(link);
  ioopm_list_iterator(link);
  ioopm_list_iterator(link);
  ioopm_list_iterator(link);

  ioopm_linked_list_clear(link);
  ioopm_linked_list_destroy(link);
}

void 
test_create_remove_at(void) 
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
test_remove_at(void)
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
      option_t result = ioopm_hash_table_remove_at(hash, randomkeys[index]);
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
test_init_array(void)
{
  array_t *arr = ioopm_array_create(4, 8, ioopm_to_int);
  ioopm_array_destroy(arr);
}

void
test_add_array_iter(void)
{
  array_t *arr = ioopm_array_create(sizeof(int), 2, ioopm_to_int);
  ioopm_iterator_t *iter = ioopm_array_iterator(arr);
  CU_ASSERT_FALSE(ioopm_iterator_has_prev(iter));
  CU_ASSERT(ioopm_iterator_has_next(iter));
  ioopm_iterator_next(iter);
  CU_ASSERT(ioopm_iterator_has_prev(iter));
  CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));



  //Clean up
  ioopm_array_destroy(arr);
  
}

void
test_edit_array_iter(void)
{
  array_t *arr = ioopm_array_create(sizeof(int), 5, ioopm_to_int);
  ioopm_iterator_t *iter = ioopm_array_iterator(arr);
  int integers[] = {0, 5, 6, 7, 8};
  int i = 0;
  do
  {
    ioopm_iterator_value_edit(iter, NULL, (void *)(integers + i));
    i++;
  } while (ioopm_iterator_next(iter));

  ioopm_iterator_reset(iter);
  i = 0;
  do
  {
    CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i,integers[i]);
    i++;
  } while (ioopm_iterator_next(iter));


  ioopm_array_destroy(arr); 
 
}

void
test_array_iter_misc(void)
{
  array_t *arr = ioopm_array_create(sizeof(int), 5, ioopm_to_int);
  ioopm_iterator_t *iter = ioopm_array_iterator(arr); 
  int integers[] = {0, 5, 6, 7, 8};
  int inc = 1;
  int i = 0;
  do
  {
    ioopm_iterator_value_edit(iter, NULL, (void *)(integers + i));
    ioopm_iterator_value_edit(iter, ioopm_increment_int, (void *) &inc);
    i++;
  } while (ioopm_iterator_next(iter));

  ioopm_iterator_reset(iter);
  i = 0;
  do
  {
    CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i,integers[i] + inc);
    i++;
  } while (ioopm_iterator_next(iter));

  ioopm_iterator_set(iter, 3);
  while (ioopm_iterator_remove_at(iter));

  ioopm_iterator_reset(iter);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 1);
  ioopm_iterator_next(iter);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 6);
  ioopm_iterator_next(iter);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 7);
  ioopm_iterator_next(iter);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 0);
  ioopm_iterator_next(iter);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 0);

  CU_ASSERT_EQUAL(ioopm_iter_db_siz(iter), 5);
  ioopm_iter_destroy_db(iter);

}

bool
ioopm_test_pred(elem_t placeholder, void *arg)
{
  return placeholder.b;
}

void
test_pipelist(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(); 
  elem_t assembler;
  elem_t assemblercontrol;
  void *generics[2] = {ioopm_test_pred, NULL};
  elem_t cast;
  cast.p = generics;
  assemblercontrol.assemble = controller;
  assembler.assemble = mover;
  elem_t to_last[] = {assembler, (elem_t) 1};
  elem_t to_first[] = {assembler, (elem_t) -1 };
  elem_t to_index[] = {assembler, (elem_t) 2, assemblercontrol, cast};

  ioopm_list_append(list, (elem_t) 1, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 2, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 3, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 4, (elem_t) NULL);
  ioopm_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_run_pipeline(iter, to_last, 1);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 4);
  ioopm_run_pipeline(iter, to_first, 1);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 1);
  ioopm_run_pipeline(iter, to_last, 1);
  ioopm_run_pipeline(iter, to_last, 1);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 4);
  ioopm_iterator_reset(iter);
  ioopm_run_pipeline(iter, to_index, 2);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 3);
  ioopm_iterator_reset(iter);
  ioopm_iterator_next(iter);
  ioopm_run_pipeline(iter, to_index, 2);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 4);

  ioopm_linked_list_clear(list);
  ioopm_linked_list_destroy(list);
}

void
test_pipe_remove_at(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  void *clean_arg1[] = {ioopm_clean_strings, NULL};
  void *clean_arg2[] = {ioopm_clean_strings, NULL};
  elem_t cleanargs;
  elem_t cleanargs2;
  cleanargs.p = clean_arg1; 
  cleanargs2.p = clean_arg2;
  elem_t assembler;
  elem_t cleaner;
  elem_t cleaner2;
  cleaner.assemble = transform_values;
  cleaner2.assemble = transform_keys;
  assembler.assemble = remover;
  elem_t remover[] = {cleaner, cleanargs, cleaner2, cleanargs2, assembler, (elem_t) NULL};
  //We need a cleaner for the key

  ioopm_list_append(list, (elem_t) strdup("1"), (elem_t) strdup("1"));
  ioopm_list_append(list, (elem_t) strdup("2"), (elem_t) strdup("1"));
  ioopm_list_append(list, (elem_t) strdup("3"), (elem_t) strdup("1"));
  ioopm_list_append(list, (elem_t) strdup("4"), (elem_t) strdup("1"));
  ioopm_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_run_pipeline(iter, remover, 3);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
  ioopm_linked_list_destroy(list);
}
void
test_pipe_inc(void)
{
  elem_t moving;
  moving.assemble = mover;
  ioopm_list_t *list = ioopm_linked_list_create();
  int inc1 = 1;
  int inc2 = 2;
  elem_t transformfirst;
  elem_t transformsec;
  transformfirst.assemble = transform_values;
  transformsec.assemble = transform_values;
  elem_t argsfirst;
  elem_t argstwo;
  void *inc_arg1[] = {ioopm_increment_int, &inc1};
  void *inc_arg2[] = {ioopm_increment_int, &inc2};
  argsfirst.p = inc_arg1;
  argstwo.p = inc_arg2;
  elem_t firstline[] = {transformfirst, argsfirst, moving, (elem_t) 1};
  elem_t secondline[] = {transformsec, argstwo, moving, (elem_t) 1};

  ioopm_list_append(list, (elem_t) 1, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 2, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 3, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 4, (elem_t) NULL);
  ioopm_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_run_pipeline(iter, firstline, 2);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 5);
  ioopm_iterator_reset(iter);
  ioopm_run_pipeline(iter, secondline, 2);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 7);
  ioopm_iterator_reset(iter);
  ioopm_run_pipeline(iter, secondline, 2);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 9);
  ioopm_iterator_prev(iter);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 8);

  ioopm_linked_list_clear(list);
  ioopm_linked_list_destroy(list);
}

bool
test_func_move_til_true(elem_t value, void *arg)
{
  void **args = arg;
  enum pipe_assemblers *assemble = args[0];
  switch (*assemble) {
    case mover:
    return value.b;
    case controller:
    return true;
    default:
    return !value.b;
  }
}
void
test_pipe_until_true(void)
{
  ioopm_list_t *list = ioopm_linked_list_create();
  elem_t first;
  elem_t sec;
  elem_t third;
  first.assemble = cmpvalue;
  sec.assemble = mover;
  third.assemble = controller;
  int compare_to = 3;
  void *cmp_arg[] = {ioopm_equals_int, &compare_to};
  void *ctrl_arg[] = {test_func_move_til_true, NULL};
  elem_t ctrl;
  ctrl.p = ctrl_arg;
  elem_t arg;
  arg.p = cmp_arg;
  elem_t line[] = {first, arg, sec, (elem_t) 1, third, ctrl};

  //ADDING STARTERS
  ioopm_list_append(list, (elem_t) 1, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 2, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 3, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 4, (elem_t) NULL);
  ioopm_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_run_pipeline(iter, line, 3);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 3);
  compare_to = 8;
  ioopm_run_pipeline(iter, line, 3);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 4);
  compare_to = 1;
  ioopm_iterator_reset(iter);
  ioopm_run_pipeline(iter, line, 3);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 1);
  //ADDING MORE THINGS
  ioopm_list_append(list, (elem_t) 17, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 68, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 5, (elem_t) NULL);
  ioopm_list_append(list, (elem_t) 9, (elem_t) NULL);

  ioopm_iterator_reset(iter);
  compare_to = 68;
  ioopm_run_pipeline(iter, line, 3);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 68);
  ioopm_iterator_reset(iter);
  compare_to = 17;
  ioopm_run_pipeline(iter, line, 3);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 17);
  ioopm_iterator_reset(iter);
  compare_to = 5;
  ioopm_run_pipeline(iter, line, 3);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 5);
  ioopm_iterator_reset(iter);
  compare_to = 9;
  ioopm_run_pipeline(iter, line, 3);
  CU_ASSERT_EQUAL(ioopm_iterator_value_at(iter).return_value.i, 9);

  ioopm_linked_list_clear(list);
  ioopm_linked_list_destroy(list);
}


int 
main(int argc, char *argv[]) 
{

  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite iter_suite = CU_add_suite("Testing iters suite", init_suite, clean_suite);
  CU_pSuite list_suite = CU_add_suite("Testing linked list", init_suite, clean_suite);
  CU_pSuite array_suite = CU_add_suite("Testing array suite", init_suite, clean_suite);
  CU_pSuite pipeline_suite = CU_add_suite("Testing pipeline suite", init_suite, clean_suite);
  CU_pSuite hash_suite = CU_add_suite("Testing hash suite", init_suite, clean_suite);
  if (hash_suite == NULL || array_suite == NULL 
      || list_suite == NULL || pipeline_suite == NULL
      || iter_suite == NULL) {
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
    || (CU_add_test(list_suite, "Linked list removal", test_link_remove_at) == NULL) 
    || (CU_add_test(list_suite, "List contains test", test_list_contains) == NULL) 
    || (CU_add_test(list_suite, "List true for all", test_list_all) == NULL) 
    || (CU_add_test(list_suite, "List incrementing", test_increment) == NULL) 
    || (CU_add_test(list_suite, "List get function", test_link_get) == NULL) 
    || (CU_add_test(list_suite, "List prepend function", test_link_prepend) == NULL) 
    || (CU_add_test(list_suite, "Create iterator over list", test_create_iterator) == NULL) 
    || (CU_add_test(list_suite, "Iter move in list", test_iterator_has_next) == NULL) 
    || (CU_add_test(list_suite, "Remove element with iterator", test_remove_at_iterator) == NULL) 
    || (CU_add_test(list_suite, "Insert and remove_at with iterator", double_iterator) == NULL) 
    || (CU_add_test(hash_suite, "create test", test_create_remove_at) == NULL) 
    || (CU_add_test(hash_suite, "Lookup test", test_lookup) == NULL) 
    || (CU_add_test(hash_suite, "insert test", test_insert) == NULL)
    || (CU_add_test(hash_suite, "insert test overwrite", test_insert_overwrite) == NULL)
    || (CU_add_test(hash_suite, "remove_at test", test_remove_at) == NULL)
    || (CU_add_test(hash_suite, "size check test", test_size) == NULL)
    || (CU_add_test(hash_suite, "Check if empty test", test_empty) == NULL)
    || (CU_add_test(hash_suite, "Clearing test", test_clear) == NULL)
    || (CU_add_test(hash_suite, "Getting keys", test_key_list) == NULL)
    || (CU_add_test(hash_suite, "Getting values", test_value_list) == NULL)
    || (CU_add_test(hash_suite, "Has key", test_has_key) == NULL)
    || (CU_add_test(hash_suite, "Has value", test_has_value) == NULL)
    || (CU_add_test(hash_suite, "Any on values", test_any) == NULL)
    || (CU_add_test(hash_suite, "Hash Stat", test_hash_stats) == NULL)
    || (CU_add_test(hash_suite, "True for all", test_all) == NULL)
    || (CU_add_test(hash_suite, "Apply to all", test_apply_all) == NULL)
    || (CU_add_test(array_suite, "Unit test: INIT array", test_init_array) == NULL)
    || (CU_add_test(array_suite, "Unit test: Iter move in array", test_add_array_iter) == NULL)
    || (CU_add_test(array_suite, "Unit test: Iter edit in array", test_edit_array_iter) == NULL)
    || (CU_add_test(array_suite, "Unit test: misc: db_siz, increment, db_destroy and remove_at", test_array_iter_misc) == NULL)
    || (CU_add_test(iter_suite, "Unit test clearing allocated iters", test_clearing_iterators) == NULL)
    || (CU_add_test(pipeline_suite, "Unit test: Iter move in pipe", test_pipelist) == NULL)
    || (CU_add_test(pipeline_suite, "Unit test: Destructer pipe", test_pipe_remove_at) == NULL)
    || (CU_add_test(pipeline_suite, "Unit test: Increment pipe", test_increment) == NULL)
    || (CU_add_test(pipeline_suite, "Unit test: Pipe until true", test_pipe_until_true) == NULL)
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
        if(!strcmp("iter", argv[i]))
        {
          CU_basic_run_suite(iter_suite);
          continue;
        }
        if(!strcmp("list", argv[i]))
        {
          CU_basic_run_suite(list_suite);
          continue;
        }
        if(!strcmp("array", argv[i]))
        {
          CU_basic_run_suite(array_suite);
          continue;
        }
        if(!strcmp("pipe", argv[i]))
        {
          CU_basic_run_suite(pipeline_suite);
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