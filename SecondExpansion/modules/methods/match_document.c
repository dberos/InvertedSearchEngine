#include"../../include/job.h"

extern Core core;
extern JobScheduler job_scheduler;

ErrorCode execute_MatchDocument(Job job){
	// Lock the mutex to process the new Document
	pthread_mutex_lock(&job_scheduler->add_document_mutex);	
	// Creating a new Document
	Document document=document_create(job->match_document_job->doc_id,
										job->match_document_job->doc_str);

	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->add_document_mutex);

	// Deduplicating the Document
	document_deduplication(document);
	// Creating the Document's Edit Distance BK-Tree from all active MT_EDIT_DIST Entries
	document_create_edit(core,document);
	// Creating the Document Hamming Distance BK-Trees from all active MT_HAMMING_DIST Entries
	document_create_hamming(core,document);

	// Matching the Document
	document_match(core,document);

	// Destroying the Job
	job_destroy(job);
	// Lock the mutex
	pthread_mutex_lock(&job_scheduler->post_mutex);
	// Insert at the Queue the available Document for GetNextAvailRes
	document_queue_insert_last(job_scheduler->document_queue,document);
	// Post semaphore so main thread knows there is available Document
	sem_post(&job_scheduler->semaphore);
	// Unlock the mutex
	pthread_mutex_unlock(&job_scheduler->post_mutex);
	return EC_SUCCESS;
}
