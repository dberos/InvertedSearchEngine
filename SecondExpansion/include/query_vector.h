#pragma once

#include"common_types.h"
#include"core.h"
#include"query.h"
#include"job_scheduler.h"

struct query_vector_node{
    Query query;
};

struct query_vector{
    QueryVectorNode array;
    atomic_int size;
    atomic_int capacity;
};

// Creating an active Query Set
QueryVector query_vector_create();

// Destroying an active Query Set
void query_vector_destroy(QueryVector vector);

// Inserting and creating a Query at a given position of an active Query Set
void query_vector_insert(QueryVector vector,QueryID query_id,String query_str,MatchType match_type,uint match_dist);

// Accesing the Query of given position
Query query_vector_at(QueryVector vector,uint pos);

// Removing a Query
void query_vector_set(QueryVector vector,uint pos);
