#include"../../include/fifoqueue.h"

FiFoQueue fifoqueue_create(){
    FiFoQueue q = (FiFoQueue)malloc(sizeof(struct fifo_queue));

    // Set size to 0
    q->size=0;
    // Set head to NULL
    q->head=NULL;
    // Set tail to NULL;
    q->tail=NULL;

    // Return the q
    return q;

}

QueueNode create_QueueNode(Job job){
    QueueNode newnode = (QueueNode)malloc(sizeof(struct queue_node));

    newnode->job = job;
    newnode->next = NULL;

    return newnode;
}


void fifoqueue_insert(FiFoQueue q, Job job){
    QueueNode newnode = create_QueueNode(job);
    
    //if queue is empty
    if(!q->size){  
        q->head = newnode;
        q->tail = newnode;
        q->size++;
        return;
    }

    //else
    q->tail->next = newnode;
    q->tail = newnode;
    q->size++;

    return;
}

QueueNode fifoqueue_pop(FiFoQueue q){

    //insert mutex here to verify safe queue multi-thread consuming

    //if the node is not empty
    if(!q->size){ printf("JobQueue was found empty!\nCannot pop from queue!\n"); return NULL;}
    //proceed
    QueueNode popnode = q->head;

    //if this was the only node left in the queue ( this is alsot he tail of the q)
    if(q->size == 1){
        q->head = NULL;
        q->tail = NULL;
        q->size--;
    
        return popnode;
    }

    //else
    q->head = q->head->next;
    q->size--;

    return popnode;

}

//Destroy a single fifo q node (we need to do this after we pop a job from the queue)
void destroy_single_fifoqueue_node(QueueNode node){
    destroy_job(node->job);
    free(node);
}

void destroy_fifoqueue_nodes(QueueNode node){
    if(node==NULL) return;

    destroy_fifoqueue_nodes(node->next);
    destroy_single_fifoqueue_node(node);
}

void fifoqueue_destroy(FiFoQueue q){
    
    //else, if there are leftover nodes(very unlikely)
    //free them
    destroy_fifoqueue_nodes(q->head);
    free(q);
    
}