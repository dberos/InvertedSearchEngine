#include"../../include/query.h"

Query query_create(QueryID id, MatchType match_type, uint match_dist){
    Query query=malloc(sizeof(*query));
    query->query_id=id;

    query->match_type=match_type;
    query->match_dist=match_dist; //the threshold
    query->lock=false;
    query->matched_words_num=0;
    query->query_words_num=0;
    query->words=malloc(sizeof(*query->words)*MAX_QUERY_WORDS);
    query->matched_words=malloc(sizeof(*query->matched_words)*MAX_QUERY_WORDS);
    // for(int i=0;i<MAX_QUERY_WORDS;i++){
    //     query->words[i]=malloc(MAX_QUERY_LENGTH); // possible only 1 word exists with MAX_LENGTH
    //     query->matched_words=malloc(MAX_QUERY_LENGTH);
    // }
    return query;
}

void addWord_to_query(Query query, String word){
    query->words[query->query_words_num] = strdup(word);
    query->query_words_num++;
}

void passWords_to_query(Query new_query, Query old_query){
    for(int i=0 ; i<old_query->query_words_num ; i++){
        new_query->words[i] = strdup(old_query->words[i]);
        new_query->query_words_num++;
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
    free(query->words);
    free(query->matched_words);
    
    free(query);
}

void printQuery(Query query){
    printf("queryprint id %u: ", query->query_id);
    for(int i=0 ; i<query->query_words_num ; i++){
        printf(" %s", query->words[i]);
    }
    printf("\n");
}