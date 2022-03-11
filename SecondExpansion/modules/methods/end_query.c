#include"../../include/job.h"

extern Core core;

ErrorCode execute_EndQuery(Job job){
	// Find the Query from the active Query Set
	Query query=query_vector_at(core->vector,job->end_query_job->query_id);
	if(query==NULL){
		return EC_FAIL;
	}
	// Get Query's MatchType
	MatchType match_type=query->match_type;
	// Remove from the correct Dictionary
	switch(match_type){
		case MT_EXACT_MATCH:
			dictionary_remove(core->exact_queries,
								core->vector,
									job->end_query_job->query_id);
			break;
		case MT_EDIT_DIST:
			dictionary_remove(core->edit_queries,
								core->vector,
									job->end_query_job->query_id);
			break;
		case MT_HAMMING_DIST:
			dictionary_remove(core->hamming_queries,
								core->vector,
									job->end_query_job->query_id);
			break;
	}
	// Destroy it
	query_vector_set(core->vector,job->end_query_job->query_id);
	// Destroy the Job
	job_destroy(job);
	return EC_SUCCESS;
}
