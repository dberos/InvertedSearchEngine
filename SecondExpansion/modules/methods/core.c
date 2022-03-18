/*
 * core.cpp version 1.0
 * Copyright (c) 2013 KAUST - InfoCloud Group (All Rights Reserved)
 * Author: Amin Allam
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include"../../include/common_types.h"
#include"../../include/core.h"
#include"../../include/methods.h"
#include"../../include/job.h"
#include"../../include/job_scheduler.h"


///////////////////////////////////////////////////////////////////////////////////////////////
Core core=NULL;
JobScheduler job_scheduler=NULL;
///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode InitializeIndex(){
	// Creating the main struct
	core=core_create();
	// Creating the Job Scheduler
	job_scheduler=job_scheduler_create(3,2,2);

	// Creating the Query threads (1st arg)
	for(int i=0;i<job_scheduler->num_query_threads;i++){
		thread_init(&job_scheduler->query_threads[i],(Pointer)query_routine);
	}
	// Creating the MatchDocument threads (2nd arg)
	for(int i=0;i<job_scheduler->num_match_threads;i++){
		thread_init(&job_scheduler->match_threads[i],(Pointer)match_routine);
	}
	// Creating the GetNextAvailRes threads (3rd arg)
	for(int i=0;i<job_scheduler->num_res_threads;i++){
		thread_init(&job_scheduler->res_threads[i],(Pointer)res_routine);
	}
	// Successful initializations
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode DestroyIndex(){
	// Lock the mutex
	pthread_mutex_lock(&job_scheduler->main_mutex);
	// Set program finish flag
	job_scheduler->fin=1;
	// Post the GetNextAvailRes threads
	for(int i=0;i<job_scheduler->num_res_threads;i++){
		sem_post(&job_scheduler->res_semaphore);
	}
	// Post Query threads
	for(int i=0;i<job_scheduler->num_query_threads;i++){
		sem_post(&job_scheduler->query_queue_semaphore);
	}
	// Signal Match threads that program is finishing
	pthread_cond_broadcast(&job_scheduler->match_queue_cond);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->main_mutex);

	// Join the Query threads
	for(int i=0;i<job_scheduler->num_query_threads;i++){
		thread_destroy(&job_scheduler->query_threads[i]);
	}
	// Join the MatchDocument threads
	for(int i=0;i<job_scheduler->num_match_threads;i++){
		thread_destroy(&job_scheduler->match_threads[i]);
	}
	// Join the GetNextAvailRes threads
	for(int i=0;i<job_scheduler->num_res_threads;i++){
		thread_destroy(&job_scheduler->res_threads[i]);
	}
	
	// Destroy the main struct
	core_destroy(core);
	// Destroy the Job Scheduler
	job_scheduler_destroy(job_scheduler);
	// Program was successfully finished
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist){
	// Lock the mutex
	pthread_mutex_lock(&job_scheduler->main_mutex);
	// Cast string to not disqualify const expression
	String str=strdup((String)query_str);
	// Creating a StartQuery Job
	StartQueryJob start_query_job=start_query_job_create(query_id,str,match_type,match_dist);
	// Creating an empty Job
	Job job=job_create();
	// Set Job
	set_start_query_job(job,start_query_job);
	// Free Query's String
	free(str);

	// Lock the mutex for safely submission
	pthread_mutex_lock(&job_scheduler->add_query_job_mutex);
	// Submit Job at the Query Queue
	queue_insert_last(job_scheduler->query_queue,job);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->add_query_job_mutex);

	// Lock the mutex to increase the Job count
	pthread_mutex_lock(&job_scheduler->jobs_mutex);
	// Increase the count of total Jobs
	job_scheduler->total_jobs++;
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->jobs_mutex);

	// Post semaphore 
	sem_post(&job_scheduler->query_queue_semaphore);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->main_mutex);
	// Successful submission
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode EndQuery(QueryID query_id){
	// Lock the mutex
	pthread_mutex_lock(&job_scheduler->main_mutex);
	// Creating an EndQuery Job
	EndQueryJob end_query_job=end_query_job_create(query_id);
	// Creating an empty Job
	Job job=job_create();
	// Set Job
	set_end_query_job(job,end_query_job);

	// Lock the mutex for safely submission
	pthread_mutex_lock(&job_scheduler->add_query_job_mutex);
	// Submit Job at the Query Queue
	queue_insert_last(job_scheduler->query_queue,job);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->add_query_job_mutex);

	// Lock the mutex to increase the Job count
	pthread_mutex_lock(&job_scheduler->jobs_mutex);
	// Increase the count of total Jobs
	job_scheduler->total_jobs++;
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->jobs_mutex);

	// Post semaphore
	sem_post(&job_scheduler->query_queue_semaphore);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->main_mutex);
	// Successful submission
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode MatchDocument(DocID doc_id, const char* doc_str){	
	// Lock the mutex
	pthread_mutex_lock(&job_scheduler->main_mutex);
	// Post semaphore
	sem_post(&job_scheduler->query_queue_semaphore);
	/**
	 * If all Start | End Query Jobs have been completed
	 * Then a Query thread will wait at barrier
	 * So does main thread, to procceed submitting MatchDocument Jobs
	 **/
	pthread_barrier_wait(&job_scheduler->match_barrier);

	// Cast String to not disqualify const expression
	String str=strdup((String)doc_str);
	// Creating a MatchDocument Job
	MatchDocumentJob match_document_job=match_document_job_create(doc_id,str);
	// Creating an empty Job
	Job job=job_create();
	// Set Job
	set_match_document_job(job,match_document_job);
	// Free Document's String
	free(str);

	// Submit Job at the Match Queue
	queue_insert_last(job_scheduler->match_queue,job);
	// Signal threads there is available Job
	pthread_cond_broadcast(&job_scheduler->match_queue_cond);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->main_mutex);
	// Successful submission
	return EC_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids){
	// Lock the mutex
	pthread_mutex_lock(&job_scheduler->main_mutex);
	// Wait at semaphore for any available document to remove from the Queue
	sem_wait(&job_scheduler->semaphore);

	// Set attributes to 0, to obtain any available Document first
	*p_doc_id=0; *p_num_res=0; *p_query_ids=0;
	// Creating a GetNextAvailRes Job
	GetNextAvailResJob get_next_avail_res_job=get_next_avail_res_job_create(p_doc_id,p_num_res,p_query_ids);
	// Creating an empty Job
	Job job=job_create();
	// Set Job
	set_get_next_avail_res_job(job,get_next_avail_res_job);

	// Lock the mutex to safely insert a Job
	pthread_mutex_lock(&job_scheduler->add_res_job_mutex);
	// Submit Job at the GetNextAvailRes Queue
	queue_insert_last(job_scheduler->res_queue,job);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->add_res_job_mutex);
	
	// Post a waiting GetNextAvailRes thread
	sem_post(&job_scheduler->res_semaphore);
	// Wait at barrier for the result
	pthread_barrier_wait(&job_scheduler->res_barrier);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->main_mutex);
	// Successful results
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
