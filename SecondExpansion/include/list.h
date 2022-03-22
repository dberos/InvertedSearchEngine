#pragma once

#include"common_types.h"
#include"core.h"
#include"job_scheduler.h"

struct list_node{
    uint value;
    ListNode next;
};

struct list{
    ListNode head;
    ListNode tail;
    atomic_int size;
};

// Creating a List
List list_create();

// Destroying a List
void list_destroy(List list);

// Inserting at the head of a List without traverse
void list_insert_head(List list,uint value);

// Inserting at the tail of a List without traverse
void list_insert_tail(List list,uint value);

// Remove a QueryID from the payload
bool list_remove(List list,uint value);

// Check the existence of a value in a list
bool check_list_existence(List list, uint value);
