#pragma once
#include "automaton.h"
//REMEMBER TO CLEAN UP INSTRUCTION SETS!
//Ioopm_list_clear


//Remember that the starting position of the iterator used
//with instruction set will change outcome

am_t *
ioopm_build_automaton(am_arr_t *am, 
                      enum tasks d_job, elem_t *args);


void
ioopm_automaton_settings(int siz, 
                         state_t *states, elem_t **arg, 
                         transition *trans_arr, bool first_time);



void
ioopm_automaton_destroy(am_arr_t *automatons);