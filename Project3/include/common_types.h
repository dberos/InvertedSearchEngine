#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<errno.h>
#include <ctype.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<sys/time.h>
#include<pthread.h>
#include<assert.h>

#define BLOOMBYTES (uint)1e5
#define BLOOMBITS (uint)8e5
#define BLOOMK 3

typedef void* Pointer;
typedef char* String;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;

typedef ulong (*HashFunction)(String);
typedef uint (*HashFunc)(Pointer);
typedef ulong (*HashBloom)(uchar*,uint);
typedef uint (*DistanceFunction)(String,int,String,int);

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
typedef struct Document* DocumentPtr;
typedef struct bloom_filter* BloomFilter;
typedef struct thread* Thread;
typedef struct job* Job;
typedef struct job_scheduler* JobScheduler;
typedef struct queue_node* QueueNode;
typedef struct fifo_queue* FiFoQueue;
typedef struct vector_node* VectorNode;
typedef struct vector* Vector;
typedef struct bkt_node* BktNode;
typedef struct bkt* Bkt;
