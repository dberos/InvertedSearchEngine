#include"../../include/methods.h"

Core core_create(){
    Core core=malloc(sizeof(*core));
    // Set NULL the map for the documents here
    // Create it in MatchDocument and destroy it in GetNextAvailableRes
    core->document=NULL;
    // Create the Dictionary for the exact matching
    core->exact_queries=dictionary_create();
    // Create the dictionary for the edit distance
    core->edit_queries=dictionary_create();
    // Create the dictionary for the hamming distance
    core->hamming_queries=dictionary_create();
    // Set 0 the number of active queries
    core->active_queries_number=0;
    return core;
}

void core_destroy(Core core){
    // Destroy all dictionaries
    dictionary_destroy(core->exact_queries);
    dictionary_destroy(core->edit_queries);
    dictionary_destroy(core->hamming_queries);
    free(core);
}
