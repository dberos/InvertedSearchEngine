#include"../../include/job.h"

extern Core core;
extern JobScheduler job_scheduler;

ErrorCode execute_GetNextAvailRes(Job job){
	// Lock the mutex to safely grab a Document
	pthread_mutex_lock(&job_scheduler->post_mutex);
	// Obtain a ready Document
	Document document=document_queue_remove_first(job_scheduler->document_queue);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->post_mutex);
	
	// Debug
	if(document==NULL){
		// Lock the mutex
		pthread_mutex_lock(&job_scheduler->barrier_mutex);
		// Wait at barrier
		pthread_barrier_wait(&job_scheduler->res_barrier);
		// Unlock the mutex
		pthread_mutex_unlock(&job_scheduler->barrier_mutex);
		return EC_NO_AVAIL_RES;
	}
	// Set attributes
	*job->get_next_avail_res_job->p_doc_id=document->doc_id;
	*job->get_next_avail_res_job->p_num_res=document->pqueue->size;

	// Allocating memory for the results
	QueryID* results=malloc(sizeof(*results)*document->pqueue->size);
	// Get a counter
	int num_res=0;
	// Insert the results at the array
	while(document->pqueue->size>=1){
		// Get the Min
		results[num_res]=pqueue_find(document->pqueue);
		// Remove it
		pqueue_remove(document->pqueue);
		// Increase the counter
		num_res++;
	}

	// Set results, they will be freed at testdriver
	*job->get_next_avail_res_job->p_query_ids=results;
	// Lock the mutex to safely insert
	pthread_mutex_lock(&job_scheduler->add_core_document_mutex);
	// Inserting the Document to the Core's Queue to destroy them all at the end
	document_queue_insert_last(core->document_queue,document);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->add_core_document_mutex);
	// Lock the mutex
	pthread_mutex_lock(&job_scheduler->barrier_mutex);
	// Wait at barrier for the result
	pthread_barrier_wait(&job_scheduler->res_barrier);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->barrier_mutex);
	// Destroy the Job
    job_destroy(job);
	return EC_SUCCESS;
}