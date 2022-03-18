#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<errno.h>
#include<ctype.h>
#include<assert.h>
#include<limits.h>
#include<math.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<sys/time.h>
#include<pthread.h>
#include<semaphore.h>

typedef void* Pointer;
typedef char* String;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;

typedef ulong (*HashFunction)(String);
typedef uint (*DistanceFunction)(String,int,String,int);

typedef enum{
    NaN,STARTQUERY,ENDQUERY,MATCHDOCUMENT,GETNEXTAVAILRES
}JobType;

typedef struct entry* Entry;
typedef struct entry_list* EntryList;
typedef struct entry_list_node* EntryListNode;
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
typedef struct Document* Document;
typedef struct vector_node* VectorNode;
typedef struct vector* Vector;
typedef struct bkt_node* BktNode;
typedef struct bkt* Bkt;
typedef struct start_query_job* StartQueryJob;
typedef struct end_query_job* EndQueryJob;
typedef struct match_document_job* MatchDocumentJob;
typedef struct get_next_avail_res_job* GetNextAvailResJob;
typedef struct job* Job;
typedef struct queue_node* QueueNode;
typedef struct queue* Queue;
typedef struct job_scheduler* JobScheduler;
typedef struct linked_list_node* LinkedListNode;
typedef struct linked_list* LinkedList;
typedef struct hash_node* HashNode;
typedef struct hash_table* HashTable;
typedef struct document_queue_node* DocumentQueueNode;
typedef struct document_queue* DocumentQueue;
typedef struct query_vector_node* QueryVectorNode;
typedef struct query_vector* QueryVector;
