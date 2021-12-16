#pragma once

#include"common_types.h"
#include"core.h"

///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all information related to an active query
struct Query
{
	QueryID query_id;
	MatchType match_type;
	unsigned int match_dist;
	
	String words[MAX_QUERY_LENGTH];
	unsigned int query_words_num;
	String matched_words[MAX_QUERY_LENGTH];
	unsigned int matched_words_num;
};

///////////////////////////////////////////////////////////////////////////////////////////////

// Creating a Query
Query query_create(QueryID id, MatchType match_type, uint match_dist);

//Add word in the array we are storing query's words
void addWord_to_query(Query query, String word);

// Destroying a Query
void query_destroy(Query query);
