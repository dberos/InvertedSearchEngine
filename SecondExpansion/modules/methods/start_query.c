#include"../../include/job.h"

extern Core core;

ErrorCode execute_StartQuery(Job job){
	// Get Query's MatchType
	MatchType match_type=job->start_query_job->match_type;
    // Get the whole Query String
    String str=strdup(job->start_query_job->query_str);
	// Each word of the String
	String word=strtok(str," ");
	// Get each word
	while(word!=NULL){
		// Insert at the correct Dictionary
		switch(match_type){
			case MT_EXACT_MATCH:
				dictionary_insert(core->exact_queries,
                					word,
                    					job->start_query_job->query_id); 
				break;
			case MT_EDIT_DIST:
				dictionary_insert(core->edit_queries,
                					word,
                    					job->start_query_job->query_id);
				break;
			case MT_HAMMING_DIST:
				dictionary_insert(core->hamming_queries,
                					word,
                    					job->start_query_job->query_id);
				break;
		}
		word=strtok(NULL," ");
	}
	free(str);
	
	// Insert at the active Query Set
	query_vector_push_back(core->vector,
								job->start_query_job->query_id,
									job->start_query_job->query_str,
										job->start_query_job->match_type,
											job->start_query_job->match_dist);
	
	job_destroy(job);
	return EC_SUCCESS;
}
