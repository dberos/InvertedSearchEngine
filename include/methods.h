#pragma once

#include"common_types.h"
#include"dictionary.h"
#include"core.h"
#include"document_queue.h"
#include"query_vector.h"


struct core{
    Dictionary exact_queries;                   // Dictionary of Exact Entries
    Dictionary edit_queries;                    // Dictionary of Edit Entries
    Dictionary hamming_queries;                 // Dictionary of Hamming Entries
    QueryVector vector;                         // Active Query Set
    DocumentQueue document_queue;               // A Queue to store all Documents
};

// Global struct to store all the needed structs
Core core_create();

// Destroy the global struct
void core_destroy(Core core);
