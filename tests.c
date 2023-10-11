#include <CUnit/Basic.h>
#include "hash.h"
#include "extended.h"
#include <stdlib.h>
#define arr_siz 100
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
      randomkeys[i].i = rand();
      ioopm_hash_table_insert(hash, randomkeys[i], sendvalues[i]);
    }

    for(int i = 0; i < arr_siz; i++)
    {
      int index = rand() % arr_siz;
      option_t result = ioopm_hash_table_remove(hash, randomkeys[index]);
      if(result.success)
      CU_ASSERT_EQUAL(result.return_value.i, sendvalues[index].i);

      CU_ASSERT_FALSE(ioopm_hash_table_lookup(hash, sendvalues[index]).success);
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
test_merch_hashtable(void)
{
  ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_merch_hash,
  ioopm_merch_cmp, no_buckets);

  merch_t m_1 = {.name = "KO", .desc = "rund", .price = 15, .hash_stock = 0};
  elem_t em_1;
  em_1.p = &m_1;
  merch_t m_2 = {.name = "OXE", .desc = "kantig", .price = 14, .hash_stock = 0};
  elem_t em_2;
  em_2.p = &m_2;
  merch_t m_3 = {.name = "OXI", .desc = "oval", .price = 13, .hash_stock = 0};
  elem_t em_3;
  em_3.p = &m_3;
  merch_t m_4 = {.name = "KOSSA", .desc = "romb", .price = 12, .hash_stock = 0};
  elem_t em_4;
  em_4.p = &m_4;
  
  elem_t merchpointer[] = {em_1, em_2, em_3, em_4};
  
  for(int i = 0; i < 4; i++)
  {
    ioopm_hash_table_insert(hash, merchpointer[i], merchpointer[i]);
  }

  for(int i = 0; i < 4; i++)
  {
    option_t value = ioopm_hash_table_lookup(hash, merchpointer[i]);
    CU_ASSERT(value.success);
  }

  ioopm_list_t *list = ioopm_hash_table_keys(hash);
  for(int i = 0; i < 4; i++)
  {
    merch_t *merch = ioopm_linked_list_get(list, i).return_value.p;
    printf("%s\n", merch->name);
  }
  ioopm_linked_list_clear(list);
  ioopm_linked_list_destroy(list);
  ioopm_hash_table_clear(hash);
  ioopm_hash_table_destroy(hash);
}

void
test_stock_hash(void)
{
  // ioopm_hash_table_t *hash = ioopm_hash_table_create(ioopm_merch_hash,
  // ioopm_merch_cmp, no_buckets);
  // ioopm_hash_add_cleaner(hash, ioopm_clean_merch);

  // merch_t *arr[25];
  // stock_value_t *stockvals[25];
  
  // for(int index = 0; index < 25; index++)
  // {
  //   char name[] = {65 + index, 0};
  //   char *string = strdup(name);
    
  //   char shelfarr[] = {65 + index, index % 10 + 48, index % 10 + 48};
  //   char *shelf = strdup(shelfarr);

  //   arr[index] = ioopm_merch_create(string, string, index); 

  //   stockvals[index] = ioopm_stock_value_create(arr[index], index);

  //   ioopm_hash_table_insert(arr[index]->hash_stock, (elem_t) shelf, (elem_t) stockvals[index]);
    
  //   elem_t m_hashed;
  //   m_hashed.p = arr[index];
  //   ioopm_hash_table_insert(hash, m_hashed, m_hashed);
  // }
  
  // for(int index = 0; index < 25; index++)
  // {
  //   elem_t get_merch;
  //   get_merch.p = arr[index];

  //   stock_value_t get_stock_value;
  //   get_stock_value.amount = stockvals[index]->amount;
  //   get_stock_value.merch = stockvals[index]->m

  //   merch_t *merch = ioopm_hash_table_lookup(hash, get_merch).return_value.p;

  //   char shelfarr[] = {65 + index, index % 10 + 48, index % 10 + 48};
  //   char *shelf = strdup(shelfarr);

  //   CU_ASSERT(ioopm_hash_table_has_key(merch->hash_stock  , (elem_t) shelf));

  //   free(shelf);
  // }
  
  // ioopm_hash_table_clear(hash);
  // ioopm_hash_table_destroy(hash);
}

int 
main() 
{
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
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
    (CU_add_test(hash_suite, "create test", test_create_remove) == NULL) 
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
    || (CU_add_test(hash_suite, "Adding shelves to merch", test_stock_hash) == NULL)
    || 0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
} 