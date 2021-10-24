#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<errno.h>

typedef void* Pointer;
typedef char* String;
typedef unsigned long ulong;

typedef ulong(*HashFunction)(String);

typedef struct word* Word;
typedef struct bst_node* BstNode;
typedef struct bst* Bst;
typedef struct map_node* MapNode;
typedef struct map* Map;

typedef enum{
    empty,occupied,deleted
} State;
