#include"../../include/query.h"
#include"../../include/methods.h"

Query query_create(QueryID id, MatchType match_type, uint match_dist){
    // Allocate memory for the Query
    Query query=malloc(sizeof(*query));
    // Set ID
    query->query_id=id;
    //  Set MatchType
    query->match_type=match_type;
    // Set MatchDistance (the threshold)
    query->match_dist=match_dist;
    // Creating a List for its words
    query->list=linked_list_create();

    // Return the Query
    return query;
}

void query_destroy(Query query){
    // Destroy the list for the words
    linked_list_destroy(query->list);
    
    // Free the Query
    free(query);
}

void query_cleanup(Query query,String query_str){
    // Get the whole Query String
    String str=strdup(query_str);
    // Each word of the Query
    String word=strtok(str," ");
    // Get each word
    while(word!=NULL){
        // Insert each word at the list
        linked_list_insert_end(query->list,word);
        word=strtok(NULL," ");
    }
    free(str);
}
