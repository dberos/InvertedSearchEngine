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
    // Creating the ID as Pointer
    query->ptr=malloc(sizeof(*query->ptr));
    *query->ptr=query->query_id;

    // Return the Query
    return query;
}

void query_destroy(Query query){
    // Destroy the list for the words
    linked_list_destroy(query->list);
    // Free the Pointer
    free(query->ptr);
    // Free the Query
    free(query);
}

void query_cleanup(Query query,String query_str){
    // Get each word
	String token;
	// Get whole Query string
	String str=strdup(query_str);
    String res=str;
	// Thread safe strtok_r
	while((token=strtok_r(res," ",&res))){
		linked_list_insert_end(query->list,token);
	}
    free(str);
}
