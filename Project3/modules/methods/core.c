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
#include"../../include/bkt.h"
#include"../../include/fifoqueue.h"



Core core=NULL;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;
pthread_cond_t cond1;
pthread_cond_t cond2;
pthread_cond_t cond3;
volatile sig_atomic_t fin=0;
volatile sig_atomic_t empty=0;
volatile sig_atomic_t signal_main=0;
pthread_mutex_t mtx;
pthread_cond_t cond_non_empty;
pthread_cond_t cond_non_full;
volatile sig_atomic_t count=0;
volatile sig_atomic_t flag_fin=0;
///////////////////////////////////////////////////////////////////////////////////////////////



int obtain(Core core){
	printf("%ld THREAD IS WAITING \n",(long)pthread_self());
	while(empty==0){
		if(flag_fin==1){
			printf("FOUND FLAG FIN == 1 \n");
			pthread_exit(0);
		}
		pthread_cond_wait(&cond1,&mutex1);
	}
	pthread_mutex_lock(&core->job_scheduler->queue_consume);
	QueueNode job_node = fifoqueue_pop(core->job_scheduler->q);
	Job current_job=NULL;
	if(job_node!=NULL){
		current_job = job_node->job;
	}
	pthread_mutex_unlock(&core->job_scheduler->queue_consume);
	
	if(current_job!=NULL){
		if(current_job->edit_job==true){
			printf("%ld GRABBED MY EDIT JOB \n",(long)pthread_self());
			SpecificMatchDocument(core, MT_EDIT_DIST, current_job->treshold);
			printf("%ld FINISHED MY EDIT JOB \n",(long)pthread_self());
		}else if(current_job->hamming_job==true){
			printf("%ld GRABBED MY HAMMING JOB \n",(long)pthread_self());
			SpecificMatchDocument(core, MT_HAMMING_DIST, current_job->treshold);
			printf("%ld FINISHED MY HAMMING JOB \n",(long)pthread_self());
		}else{
			printf("%ld GRABBED MY EXACT JOB \n",(long)pthread_self());
			SpecificMatchDocument(core, MT_EXACT_MATCH, current_job->treshold);
			printf("%ld FINISHED MY EXACT JOB \n",(long)pthread_self());
		}
	}
	else{
		signal_main=1;
		return 1;
	}
	return 0;
}

Pointer consumer(Core core){
	while(fin==0){
		int num=obtain(core);
		if(num==1){
			pthread_mutex_unlock(&mutex2);
			pthread_cond_signal(&cond2);
		}
	}
	return 0;
}

ErrorCode InitializeIndex(){
	core=core_create();
	pthread_mutex_init(&mutex1,0);
	pthread_mutex_init(&mutex2,0);
	pthread_mutex_init(&mutex3,0);
	pthread_mutex_init(&mtx,0);
	pthread_cond_init(&cond1,0);
	pthread_cond_init(&cond2,0);
	pthread_cond_init(&cond3,0);
	pthread_cond_init(&cond_non_empty,0);
	pthread_cond_init(&cond_non_full,0);
	
	pthread_mutex_lock(&mutex1);

	for(int i=0;i<core->job_scheduler->num_threads;i++){
		thread_init(&core->job_scheduler->threads[i],consumer);
	}
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode DestroyIndex(){
	fin=1;
	flag_fin=1;
	pthread_mutex_lock(&mutex3);
	pthread_mutex_unlock(&mutex1);
	pthread_cond_broadcast(&cond1);
	pthread_mutex_unlock(&mutex3);
	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);
	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&cond1);
	pthread_cond_destroy(&cond2);
	pthread_cond_destroy(&cond3);
	pthread_cond_destroy(&cond_non_empty);
	pthread_cond_destroy(&cond_non_full);

	for(int i=0;i<core->job_scheduler->num_threads;i++){
		thread_destroy(&core->job_scheduler->threads[i]);
	}


	core_destroy(core);
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist){
	// Cast to String to not disqualify const expression
    String str=strdup((String)query_str);
	// Each word of the String
	String word=strtok(str," ");
	// Get each word
	while(word!=NULL){
		// Clean each word
		remove_special_characters_decapitalize(word);
		// Insert at the MatchType Dictionary
		if(match_type==MT_EXACT_MATCH){
			dictionary_insert(core->exact_queries, word,query_id); 
		} 
		else if(match_type==MT_EDIT_DIST){
			dictionary_insert(core->edit_queries, word,query_id);
		} 
		else{
			dictionary_insert(core->hamming_queries, word,query_id);
		}

		word=strtok(NULL," ");
	}
	free(str);
	
	// Insert at the MatchType[match_dist] QueryMap
	// The cleaning of each word of the whole query happens at QueryMap
	if(match_type==MT_EXACT_MATCH){
		query_map_insert(core->query_exact_map,query_id,(String)query_str,match_type,match_dist);
	}
	else if(match_type==MT_EDIT_DIST){

		query_map_insert(core->query_edit_map[match_dist],query_id,(String)query_str,match_type,match_dist);
	}
	else{
		query_map_insert(core->query_hamming_map[match_dist],query_id,(String)query_str,match_type,match_dist);
	}

	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode EndQuery(QueryID query_id){
	Query query=NULL;
	// Start from the only EXACT MATCH QueryMap
	query=query_map_find(core->query_exact_map,query_id);
	// If Query was at EXACT MATCH QueryMap
	if(query!=NULL){
		// Remove the Entries
		dictionary_remove(core->exact_queries,core->query_exact_map,query_id);
		// Remove the Query
		query_map_remove(core->query_exact_map,query_id);
		return EC_SUCCESS;
	}
	else{
		// Find Query from the MatchType[match_dist] active Query set
		for(int i=0;i<4;i++){
			// Check whether Query is active at Edit Distance QueryMap[i]
			query=query_map_find(core->query_edit_map[i],query_id);
			if(query!=NULL){
				// Remove the Entries
				dictionary_remove(core->edit_queries,core->query_edit_map[i],query_id);
				// Remove the Query
				query_map_remove(core->query_edit_map[i],query_id);
				return EC_SUCCESS;
			}
			// Check whether Query is active at Hamming Distance QueryMap[i]
			query=query_map_find(core->query_hamming_map[i],query_id);
			if(query!=NULL){
				// Remove the Entries
				dictionary_remove(core->hamming_queries,core->query_hamming_map[i],query_id);
				// Remove the Query
				query_map_remove(core->query_hamming_map[i],query_id);
				return EC_SUCCESS;
			}
		}
	}
	return EC_FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////
ErrorCode MatchDocument(DocID doc_id, const char* doc_str){
	
	core->document = map_create();
	// Create document (so we can save its info/results)
	core->current_doc = addDocument(core, doc_id);
	
	// Deduplicate text & store its words in a hashtable
	String str=strdup((String)doc_str);
	

	dedup(str, core);
	free(str);

	// Create edit distance BK-Tree
	core->current_edit_tree = malloc(sizeof(*core->current_edit_tree));
    build_entry_index(core->document->entry_list, MT_EDIT_DIST, core->current_edit_tree);

	// Create hamming distance BK-Trees for ever word length found in this document
	core->current_hamming_array = malloc(28*sizeof(*core->current_hamming_array));
	fill_hamming_ix_array(core->current_hamming_array, core->document->entry_list, MT_HAMMING_DIST);

//-------------------------------------------------------------------------------------



	//So the process is:

//-------------------------------------------------------------------------------------
	
	// 1) Add the jobs that need to be done to Jobs FIFO Queue

	//	Edit distance queries
	submit_job(core->job_scheduler, create_job(true, false, false, 0));	// threshold 0
	submit_job(core->job_scheduler, create_job(true, false, false, 1));	// threshold 1
	submit_job(core->job_scheduler, create_job(true, false, false, 2));	// threshold 2
	submit_job(core->job_scheduler, create_job(true, false, false, 3));	// threshold 3

	//	Hamming distance queries
	submit_job(core->job_scheduler, create_job(false, true, false, 0));	// threshold 0
	submit_job(core->job_scheduler, create_job(false, true, false, 1));	// threshold 1
	submit_job(core->job_scheduler, create_job(false, true, false, 2));	// threshold 2
	submit_job(core->job_scheduler, create_job(false, true, false, 3));	// threshold 3

	//	Exact match queries
	submit_job(core->job_scheduler, create_job(false, false, true, 0));	// threshold 0

	//	End jobs (as many as our threads are)
	// if a thread is assigned an End Job then it has to stop listening to the FIFO job queue
	// for new jobs and terminate itself

	pthread_mutex_lock(&mutex2);
	empty=1;
	pthread_cond_signal(&cond1);
	pthread_mutex_unlock(&mutex1);
	while(signal_main==0){
		printf("I AM THE MAIN THREAD %ld AND WAITING FOR MY THREADS TO FINISH THEIR JOBS \n",(long)pthread_self());
		pthread_cond_wait(&cond2,&mutex2);
	}
	pthread_mutex_lock(&mutex2);
	printf("MY THREADS HAVE FINISHED THEIR JOBS \n");
	signal_main=1;
	empty=0;
	pthread_mutex_unlock(&mutex2);


	// 4) Destroy the structures created from thsi document
	clear_matchedInfo(core);

	destroy_entry_index(core->current_edit_tree);
	destroy_hamming_array(core->current_hamming_array);

	map_destroy(core->document);
	printf("MATCH DOCUMENT HAS FINISHED \n");

	pthread_mutex_lock(&mutex1);

	return EC_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////////////////////

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

ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res, QueryID** p_query_ids){
	*p_doc_id=0; *p_num_res=0; *p_query_ids=0;

	if(core->document_number==0) return EC_NO_AVAIL_RES;

	*p_doc_id = core->docs[core->last_result_index]->doc_id;
	*p_num_res = core->docs[core->last_result_index]->num_res;
	mergeSort(core->docs[core->last_result_index]->query_ids, 0, core->docs[core->last_result_index]->num_res-1);
	*p_query_ids = core->docs[core->last_result_index]->query_ids;

	core->last_result_index++;
	
	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
