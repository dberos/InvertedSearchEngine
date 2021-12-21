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
    // Create the active Query Set
    core->query_map=query_map_create();
    //Number of documents saved in Document array
    core->document_number=0;
    core->docs=NULL;
    core->last_result_index=0;

    for(int i=0 ; i<4 ; i++){
        core->th_boxes[i]=query_list_create();
    }
    
    return core;
}

void core_destroy(Core core){
    // Destroy all dictionaries
    dictionary_destroy(core->exact_queries);
    dictionary_destroy(core->edit_queries);
    dictionary_destroy(core->hamming_queries);
    // Destroy active Query Set
    query_map_destroy(core->query_map);

    //Destroy th_boxes
    for(int i=0 ; i<4 ; i++){
        query_list_destroy(core->th_boxes[i]);
    }
    
    destroyDocumentsArray(core->docs, core->document_number);
    free(core);
}
