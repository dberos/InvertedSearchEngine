#include"../../include/query.h"

Query query_create(QueryID id,String str,MatchType match_type,uint match_dist){
    Query query=malloc(sizeof(*query));
    query->query_id=id;
    strcpy(query->str,str);
    query->match_type=match_type;
    query->match_dist=match_dist;
    return query;
}

void query_destroy(Query query){
    free(query);
}
