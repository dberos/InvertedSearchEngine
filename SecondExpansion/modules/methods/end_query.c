#include"../../include/job.h"

extern Core core;
extern JobScheduler job_scheduler;

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

	// Destroy the Job
	job_destroy(job);
	return EC_SUCCESS;
}
