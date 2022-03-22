#pragma once

#include"common_types.h"
#include"document.h"


struct document_queue_node{
    DocumentQueueNode next;
    Document document;
};

struct document_queue{
    DocumentQueueNode head;
    DocumentQueueNode tail;
    atomic_int size;
};

// Creating a Queue
DocumentQueue document_queue_create();

// Destroying a Queue
void document_queue_destroy(DocumentQueue queue);

// Inserting at the end of a Queue
void document_queue_insert_last(DocumentQueue queue,Document document);

// Removing at the head of a Queue
Document document_queue_remove_first(DocumentQueue queue);

// Get the Last Inserted document of the Queue
Document document_queue_last(DocumentQueue queue);
