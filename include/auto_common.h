#pragma once
#include "common.h"
#include "linked_list.h"
#include "automaton.h"

struct state {
    transition s_transiton;
    mem_clean m_clean;

    size_t mem_index;
    elem_t **mem;
};

