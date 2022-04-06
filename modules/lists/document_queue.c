#include"../../include/document_queue.h"

DocumentQueue document_queue_create(){
    // Allocating memory for the Queue
    DocumentQueue queue=malloc(sizeof(*queue));

    // Set head to NULL
    queue->head=NULL;
    // Set tail to NULL
    queue->tail=NULL;
    // Set size to 0
    queue->size=0;

    // Return the Queue
    return queue;
}

void document_queue_destroy(DocumentQueue queue){
    // Destroy everything 
    DocumentQueueNode prev;
    DocumentQueueNode curr;
    curr=queue->head;
    while(curr!=NULL){
        prev=curr;
        curr=curr->next;
        if(prev->document!=NULL){
            document_destroy(prev->document);
        }
        free(prev);
        queue->size--;
    }
    free(queue);
}

void document_queue_insert_last(DocumentQueue queue,Document document){
    // Creating a new Node
    DocumentQueueNode node=malloc(sizeof(*node));
    // Set the Document of the node
    node->document=document;
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

Document document_queue_remove_first(DocumentQueue queue){
    // Node to be removed is head
    DocumentQueueNode node=queue->head;
    // Check whether head exists
    if(node==NULL){
        return NULL;
    }

    // Document to be returned is head's Document
    Document document=node->document;
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
    // Return the Document
    return document;
}

Document document_queue_last(DocumentQueue queue){
    return queue->tail!=NULL ? queue->tail->document : NULL;
}