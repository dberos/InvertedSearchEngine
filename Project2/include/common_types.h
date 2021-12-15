#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<errno.h>
#include <ctype.h>

typedef void* Pointer;
typedef char* String;
typedef unsigned long ulong;
typedef unsigned int uint;

typedef ulong (*HashFunction)(String);
typedef uint (*HashFunc)(Pointer);

typedef struct word* Word;
typedef struct entry* Entry;
typedef struct entry_list* EntryList;
typedef struct entry_list_node* EntryListNode;
typedef struct word_list_node* WordListNode;
typedef struct word_list* WordList;
typedef struct dictionary_node* DictionaryNode;
typedef struct dictionary* Dictionary;
typedef struct bst_node* BstNode;
typedef struct bst* Bst;
typedef struct map_node* MapNode;
typedef struct map* Map;
typedef struct list_node* ListNode;
typedef struct list* List;
typedef struct core* Core;
typedef struct Query* Query;
typedef struct query_map_node* QueryMapNode;
typedef struct query_map* QueryMap;
typedef struct query_list_node* QueryListNode;
typedef struct query_list* QueryList;
