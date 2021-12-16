#include"../../include/query.h"

Query query_create(QueryID id, MatchType match_type, uint match_dist){
    Query query=malloc(sizeof(*query));
    query->query_id=id;

    query->match_type=match_type;
    query->match_dist=match_dist; //the threshold
    query->matched_words_num=0;
    query->query_words_num=0;
    return query;
}

void addWord_to_query(Query query, String word){
    query->words[query->query_words_num++] = strdup(word);
}

void passWords_to_query(Query new_query, Query old_query){
    for(int i=0 ; i<old_query->query_words_num ; i++){
        new_query->words[i] = strdup(old_query->words[i]);
    }
}

void query_destroy(Query query){
    int i;

    for(i=0 ; i<query->query_words_num ; i++){
        free(query->words[i]);
    }

    for(i=0 ; i<query->matched_words_num ; i++){
        free(query->matched_words[i]);
    }
    
    free(query);
}
