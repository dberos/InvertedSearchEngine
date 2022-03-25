#pragma once

#include"common_types.h"
#include"core.h"

struct priority_queue_node{
    QueryID id;
};

struct priority_queue{
    PriorityQueueNode array;
    int size;
    int capacity;
    Priority priority;
};

// Creating a Priority Queue
PriorityQueue pqueue_create(Priority priority);

// Destroying a Priority Queue
void pqueue_destroy(PriorityQueue pqueue);

// Get the PriorityQueueNode
PriorityQueueNode pqueue_at(PriorityQueue pqueue,int pos);

// Get the id of a PriorityQueueNode
QueryID pqueue_get(PriorityQueue pqueue,int pos);

// Set the id of a PriorityQueueNode
void pqueue_set(PriorityQueue pqueue,QueryID id,int pos);

// Swap keys of 2 Nodes
void pqueue_swap(PriorityQueue pqueue,int pos1,int pos2);

// Bubble Up
void pqueue_bubble_up(PriorityQueue pqueue,int pos);

// Bubble Down
void pqueue_bubble_down(PriorityQueue,int pos);

// Insert an id at the Priority Queue
void pqueue_insert(PriorityQueue pqueue,QueryID id);

// Remove from the Priority Queue MAX / MIN depending on Priority
void pqueue_remove(PriorityQueue pqueue);

// Find MAX / MIN depending on Priority
QueryID pqueue_find(PriorityQueue pqueue);
