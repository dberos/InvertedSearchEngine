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

void query_destroy(Query query){
    free(query);
}
