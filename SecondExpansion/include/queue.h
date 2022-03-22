#pragma once

#include"common_types.h"
#include"job.h"

struct queue_node{
    QueueNode next;
    Job job;
};

struct queue{
    QueueNode head;
    QueueNode tail;
    atomic_int size;
};

// Creating a Queue
Queue queue_create();

// Destroying a Queue
void queue_destroy(Queue queue);

// Inserting at the end of a Queue
void queue_insert_last(Queue queue,Job job);

// Removing at the head of a Queue
Job queue_remove_first(Queue queue);
