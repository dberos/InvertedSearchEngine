#pragma once

#include"common_types.h"
#include"job_scheduler.h"

//This is a FIFO Queue of Jobs implementation

struct queue_node{
    Job job;

    QueueNode next;    
};

struct fifo_queue{
    QueueNode head;
    QueueNode tail;

    int size;

};


// Create the fifo q
FiFoQueue fifoqueue_create();

// Create a fifo q node
QueueNode create_QueueNode(Job job);

// Insert a new Job in the fifo q
void fifoqueue_insert(FiFoQueue q, Job job);

// Pop a node (a Job) from the fifo queue
// safe multi-thread queue consuming ( contains mutex )
QueueNode fifoqueue_pop(FiFoQueue q);

// Destroy a single fifo q node (we need to do this after we pop a job from the queue)
void destroy_single_fifoqueue_node(QueueNode node);

// Destroy all the fifo q nodes (in case there are jobs left when we want to destroy a q (very unlikely, just to be safe))
void destroy_fifoqueue_nodes(QueueNode node);

// Destroy the fifo q
void fifoqueue_destroy(FiFoQueue q);


