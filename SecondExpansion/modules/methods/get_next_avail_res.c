#include"../../include/job.h"

extern Core core;
extern JobScheduler job_scheduler;

void merge(QueryID queryId[], int left, int mid, int right) {
    int lsubLen = mid - left + 1;
    int rsubLen = right - mid;
  
	/* Create sub arrays where:
		leftSub = queryId[0, mid] 
		rightSub = queryId[mid + 1, right] */
    int leftSub[lsubLen], rightSub[rsubLen];
    for (int i = 0; i < lsubLen; i++) leftSub[i] = queryId[left + i];
    for (int j = 0; j < rsubLen; j++) rightSub[j] = queryId[mid + 1 + j];
  
	// Merge array back until we reach the end of at least one array
    int i = 0;
    int j = 0;
    int k = left;
    while (i < lsubLen && j < rsubLen) {
        if (leftSub[i] <= rightSub[j]) {
            queryId[k] = leftSub[i];
            i++;
        }
        else {
            queryId[k] = rightSub[j];
            j++;
        }
        k++;
    }
  
	// Finally if one of the two arrays has remaining
	// queries add them to the original
    while (i < lsubLen) queryId[k++] = leftSub[i++];
    while (j < rsubLen) queryId[k++] = rightSub[j++];
}
  
// Mergesort breaks each given array into 2 parts (in the middle)
// sorting the children, and then "merging" the sorted arrays back up
void mergeSort(QueryID queryId[], int left, int right) {
    if (left < right) {
        int m = left + (right - left) / 2;
  
        mergeSort(queryId, left, m);
        mergeSort(queryId, m + 1, right);
  
        merge(queryId, left, m, right);
    }
}

ErrorCode execute_GetNextAvailRes(Job job){
	// Lock the mutex to safely grab a Document
	pthread_mutex_lock(&job_scheduler->post_mutex);
	// Obtain a ready Document
	Document document=document_queue_remove_first(job_scheduler->document_queue);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->post_mutex);
	
	// Debug
	if(document==NULL){
		pthread_mutex_lock(&job_scheduler->barrier_mutex);
		pthread_barrier_wait(&job_scheduler->res_barrier);
		pthread_mutex_unlock(&job_scheduler->barrier_mutex);
		return EC_NO_AVAIL_RES;
	}
	// Set attributes
	*job->get_next_avail_res_job->p_doc_id=document->doc_id;
	*job->get_next_avail_res_job->p_num_res=document->num_res;

	// Allocating memory for the results
	QueryID* results=malloc(sizeof(*results)*document->num_res);
	// Insert the results at the array
	for(int i=0;i<document->num_res;i++){
		results[i]=document->query_ids[i];
	}
	// Sort the array of the results
	mergeSort(results,0,document->num_res-1);

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