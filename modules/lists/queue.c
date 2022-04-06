#include"../../include/queue.h"

Queue queue_create(){
    // Allocating memory for the Queue
    Queue queue=malloc(sizeof(*queue));

    // Set head to NULL
    queue->head=NULL;
    // Set tail to NULL
    queue->tail=NULL;
    // Set size to 0
    queue->size=0;

    // Return the Queue
    return queue;
}

void queue_destroy(Queue queue){
    // Destroy everything 
    QueueNode prev;
    QueueNode curr;
    curr=queue->head;
    while(curr!=NULL){
        prev=curr;
        curr=curr->next;
        if(prev->job!=NULL){
            job_destroy(prev->job);
        }
        free(prev);
        queue->size--;
    }
    free(queue);
}

void queue_insert_last(Queue queue,Job job){
    // Creating a new Node
    QueueNode node=malloc(sizeof(*node));
    // Set the job of the Node
    node->job=job;
    // Set next to NULL
    node->next=NULL;

    // If Queue is empty
    if(queue->tail==NULL){
        queue->head=node;
        queue->tail=node;
    }
    else{
        queue->tail->next=node;
        queue->tail=node;
    }
    queue->size++;
}

Job queue_remove_first(Queue queue){
    // Node to be removed is head
    QueueNode node=queue->head;
    // Check whether head exists
    if(node==NULL){
        return NULL;
    }
    // Job to be be return is head's job
    Job job=node->job;
    // Change head pointer
    queue->head=queue->head->next;
    // Check whether queue got empty
    if(queue->head==NULL){
        queue->tail=NULL;
    }

    // Decrease the size
    queue->size--;
    // Free the Node
    free(node);
    // Return the Job
    return job;
}
