#pragma once

#include"common_types.h"
#include"core.h"

///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all information related to an active query
struct Query
{
	QueryID query_id;
	char str[MAX_QUERY_LENGTH];
	MatchType match_type;
	unsigned int match_dist;
};

///////////////////////////////////////////////////////////////////////////////////////////////

// Creating a Query
Query query_create(QueryID id,String str,MatchType match_type,uint match_dist);

// Destroying a Query
void query_destroy(Query query);
