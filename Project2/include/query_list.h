#pragma once

#include"common_types.h"
#include"core.h"
#include"query.h"

struct query_list_node{
    Query query;
    QueryListNode next;
};

struct query_list{
    QueryListNode head;
    QueryListNode tail;
    int size;
};

// Creating a List
QueryList query_list_create();

// Destroying a List
void query_list_destroy(QueryList query_list);

// Inserting at the head of a List without traverse
void query_list_insert_head(QueryList query_list,Query query);

// Inserting at the tail of a List without traverse
void query_list_insert_tail(QueryList query_list,Query query);

// Remove a Query from the List based on its id
bool query_list_remove(QueryList list,Pointer id);