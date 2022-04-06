#include"../../include/methods.h"

Core core_create(){
    // Allocate memory for the Core
    Core core=malloc(sizeof(*core));

    // Create the Dictionary for the exact matching
    core->exact_queries=dictionary_create();
    // Create the dictionary for the edit distance
    core->edit_queries=dictionary_create();
    // Create the dictionary for the hamming distance
    core->hamming_queries=dictionary_create();
    // Create an active Query Set
    core->vector=query_vector_create();
    // Create a Queue to store all Documents
    core->document_queue=document_queue_create();
    
    // Return the Core
    return core;
}

void core_destroy(Core core){
    // Destroy all Dictionaries
    dictionary_destroy(core->exact_queries);
    dictionary_destroy(core->edit_queries);
    dictionary_destroy(core->hamming_queries);
    // Destroy active Query Set
    query_vector_destroy(core->vector);

    // Destroying the Documents
    // They have been processed from the Job Scheduler's Queue
    document_queue_destroy(core->document_queue);
    
    // Free the Core
    free(core);
}
