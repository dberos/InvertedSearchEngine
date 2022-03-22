#pragma once

#include"common_types.h"

struct linked_list_node{
    LinkedListNode next;
    String string;
};

struct linked_list{
    LinkedListNode head;
    atomic_int size;
};

// Creating a Linked List
LinkedList linked_list_create();

// Destroying a Linked List
void linked_list_destroy(LinkedList linked_list);

// Inserting a word at the end of a Linked List
bool linked_list_insert_end(LinkedList linked_list,String string);
