#include"../../include/job.h"

extern Core core;
extern JobScheduler job_scheduler;

ErrorCode execute_StartQuery(Job job){
	// Get Query's MatchType
	MatchType match_type=job->start_query_job->match_type;
    // Get the whole Query String
    String str=strdup(job->start_query_job->query_str);
	// Free issues without that
	String res=str;
	// Get each word
	String token;
	// Thread safe strtok_r
	while((token=strtok_r(res," ",&res))){
		// Insert at the correct Dictionary
		switch(match_type){
			case MT_EXACT_MATCH:
				dictionary_insert(core->exact_queries,
                					token,
                    					job->start_query_job->query_id); 
				break;
			case MT_EDIT_DIST:
				dictionary_insert(core->edit_queries,
                					token,
                    					job->start_query_job->query_id);
				break;
			case MT_HAMMING_DIST:
				dictionary_insert(core->hamming_queries,
                					token,
                    					job->start_query_job->query_id);
				break;
		}
	}


	free(str);
	
	// Insert at the active Query Set
	query_vector_insert(core->vector,
								job->start_query_job->query_id,
									job->start_query_job->query_str,
										job->start_query_job->match_type,
											job->start_query_job->match_dist);

	// Lock the mutex to increase the count of finished Jobs
	pthread_mutex_lock(&job_scheduler->jobs_mutex);
	// Increase the count
	job_scheduler->finished_jobs++;
	// Check whether Total = Finished
	if(job_scheduler->total_jobs==job_scheduler->finished_jobs){
		// Many times will be the same, but the last signal will count
		pthread_cond_signal(&job_scheduler->res_queue_cond);
	}
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->jobs_mutex);

	job_destroy(job);
	return EC_SUCCESS;
}
