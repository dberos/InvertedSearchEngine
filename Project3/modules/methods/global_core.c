#include"../../include/methods.h"
#include"../../include/map.h"


Core core_create(){
    Core core=malloc(sizeof(*core));
    // Set NULL the map for the documents here

    // Create the Dictionary for the exact matching
    core->exact_queries=dictionary_create();
    // Create the dictionary for the edit distance
    core->edit_queries=dictionary_create();
    // Create the dictionary for the hamming distance
    core->hamming_queries=dictionary_create();
    // Set 0 the number of active queries
    core->active_queries_number=0;
    // Create the active Query Set
    // core->query_map=query_map_create();
    core->query_exact_map=query_map_create();
    core->query_edit_map=malloc(sizeof(*core->query_edit_map)*4);
    core->query_hamming_map=malloc(sizeof(*core->query_hamming_map)*4);
    for(int i=0;i<4;i++){
        core->query_edit_map[i]=query_map_create();
        core->query_hamming_map[i]=query_map_create();
    }
    //Number of documents saved in Document array
    core->document_number=0;
    core->docs=NULL;
    core->last_result_index=0;

    core->finished_jobs = false;
    

    core->job_scheduler=job_scheduler_create(THREADS_NUMBER);
    
    return core;
}

void core_destroy(Core core){
    // Destroy all dictionaries
    dictionary_destroy(core->exact_queries);
    dictionary_destroy(core->edit_queries);
    dictionary_destroy(core->hamming_queries);
    // Destroy active Query Set
    // query_map_destroy(core->query_map);
    query_map_destroy(core->query_exact_map);
    for(int i=0;i<4;i++){
        query_map_destroy(core->query_edit_map[i]);
        query_map_destroy(core->query_hamming_map[i]);
    }
    free(core->query_edit_map);
    free(core->query_hamming_map);

    
    destroyDocumentsArray(core->docs, core->document_number);
    job_scheduler_destroy(core->job_scheduler);
    free(core);
}


//-------------------------------------------------------------------------------

///DocInfo struct (@methods.h)

DocInfo create_docinfo(){
    DocInfo newdoc = (DocInfo)malloc(sizeof(struct docinfo));

    newdoc->document = map_create();

    newdoc->exact_queries = NULL;
    newdoc->edit_queries = NULL;
    newdoc->hamming_queries = NULL;

    newdoc->active_queries_number = 0;

    newdoc->query_exact_map = NULL;
    newdoc->query_edit_map = (QueryMap*) malloc(sizeof(QueryMap)*4);
    newdoc->query_hamming_map = (QueryMap*) malloc(sizeof(QueryMap)*4);

    newdoc->doc = NULL;

    // newdoc->edit_tree = (Index_ptr)malloc(sizeof(struct bkindex));
    // newdoc->hamming_array = (Index_ptr*)malloc(28*sizeof(Index_ptr));

    return newdoc;
}
