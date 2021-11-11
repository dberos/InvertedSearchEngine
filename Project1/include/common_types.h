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

typedef ulong (*HashFunction)(String);

typedef struct word* Word;
typedef struct entry* Entry;
typedef struct word_list_node* WordListNode;
typedef struct word_list* WordList;
typedef struct dictionary* Dictionary;
typedef struct bst_node* BstNode;
typedef struct bst* Bst;
typedef struct map_node* MapNode;
typedef struct map* Map;
