#pragma once

#include"common_types.h"
#include"core.h"

struct list_node{
    Pointer value;
    ListNode next;
};

struct list{
    ListNode head;
    ListNode tail;
    int size;
};

// Creating a List
List list_create();

// Destroying a List
void list_destroy(List list);

// Inserting at the head of a List without traverse
void list_insert_head(List list,Pointer value);

// Inserting at the tail of a List without traverse
void list_insert_tail(List list,Pointer value);
