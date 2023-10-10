# BUILD
To use this as a library, use make hash_table.so

To test this library and potential changes to it run make

# DOCUMENTATION

## hash
Uses linked_list pointer array to hold elements as nodes.

## linked_list
Uses nodes as elements and a list struct as header.
This list is doubly linked, so each node has a previous and next node.
The header links to the first and last dummy node.
Linked list also has keys but they don't have a lot of implementation yet

## iterator
Iterator currently only for linked_list

## common
Contains datatypes and functions used by all the other files. For example, the elem_t union over common datatypes.

## extended

For users who wish to use the hash table, extended will be very important, it is here you can implement your custom datatypes
and hashing and compare functions for those. Remember to use the void pointer of elem_t for these to work in the hash table.