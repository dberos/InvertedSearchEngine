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


static Pointer trial(Core core){




	while(1){

		//Safely grab a job from the FIFO job queue!!!
		//wait untilthe queue has something to grab
		sem_wait(&core->job_scheduler->emptyQ);
		pthread_mutex_lock(&core->job_scheduler->fifoqueue_mtx);

		//Consume
		QueueNode job_node = fifoqueue_pop(core->job_scheduler->q);
		Job current_job = job_node->job;



		pthread_mutex_unlock(&core->job_scheduler->fifoqueue_mtx);

		//Check whether this is a finish job ( check GetNextAvailRes() for explannation)
		if(current_job->finish_job==true){
			// printf("FINISHJOB\n");
			//wait until THREADNUMBER + 2 (main thread) processes get here
			//the main thread process is waiting for the threads to get here
			pthread_barrier_wait(&core->job_scheduler->resultsbarrier);
			destroy_end_or_finish_job(current_job);
			free(job_node);

			continue;
		}

		//Check whether this is an end job
		// if a thread is assigned an End Job then it has to stop listening to the FIFO job queue
		// for new jobs and terminate itself
		if(current_job->end_job==true){
			destroy_end_or_finish_job(current_job);
			free(job_node);

			break;
		}


		//Match Document
		// pthread_mutex_lock(&core->job_scheduler->stupidmtx);

		JobMatchDocument(current_job, current_job->document->doc->doc_id);
		// pthread_mutex_unlock(&core->job_scheduler->stupidmtx);

		// printf("Finished working on doc_id %u thraed number %ld \n", current_job->document->doc->doc_id, (long)pthread_self());


		// Destroy current_job
		destroy_job(current_job);
		free(job_node);
	}

	
	return NULL;
}

ErrorCode InitializeIndex(){
	core=core_create();
	// Initialize threads that will do the jobs
	for(int i=0;i<core->job_scheduler->num_threads;i++){
		thread_init(&core->job_scheduler->threads[i],trial);
	}

	return EC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode DestroyIndex(){
	for(int i=0;i<core->job_scheduler->num_threads;i++){

		pthread_mutex_lock(&core->job_scheduler->fifoqueue_mtx);

		//produce
		submit_job(core->job_scheduler, create_End_job());

		pthread_mutex_unlock(&core->job_scheduler->fifoqueue_mtx);
		sem_post(&core->job_scheduler->emptyQ); //Notify that something got into the queue

	}

	//Wait for threads to end their work
	for(int i=0;i<core->job_scheduler->num_threads;i++){
        thread_destroy(&core->job_scheduler->threads[i]);
    }
	// printf("Destroy!\n");
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
	
	core->finished_jobs = false; // This will be explained when we get to GetNextAvailRes
//-------------------------------------------------------------------------------------


	// 1) Create a new job (@job_scheduler.h for job description/explanation)

	Job newjob = create_job();

//-------------------------------------------------------------------------------------


	// 2) Fill the job attributes
	
	// -deduplicate document text and store it in the hashtable
	String str=strdup((String)doc_str);
	dedup(str, newjob->document);
	free(str);
	// if(doc_id==3) print_entrylist(newjob->document->document->entry_list);

	//Create the BK - trees
	// build_entry_index(core, newjob->document->document->entry_list, MT_EDIT_DIST, newjob->document->edit_tree);
	// fill_hamming_ix_array(core, newjob->document->hamming_array, newjob->document->document->entry_list, MT_HAMMING_DIST);

	// if(doc_id == 3) printbk(newjob->document->edit_tree->root);

	// -copy the dictionaries of the current active queries and store them
	newjob->document->exact_queries = dictionary_copy_create(core->exact_queries);
	newjob->document->edit_queries = dictionary_copy_create(core->edit_queries);
	newjob->document->hamming_queries = dictionary_copy_create(core->hamming_queries);

	// -drain active queries number
	newjob->document->active_queries_number = core->active_queries_number;
	
	// -create document (so we can save its info/results)
	newjob->document->doc = addDocument(core, doc_id);
	

	// -copy the query maps of ordered queries - These sets have the queries ordered by threshold (match_dist) number (0/ 1/ 2/ 3)
	newjob->document->query_exact_map = query_map_copy_create(core->query_exact_map);
	for(int i=0;i<4;i++){
        newjob->document->query_edit_map[i]=query_map_copy_create(core->query_edit_map[i]);
        newjob->document->query_hamming_map[i]=query_map_copy_create(core->query_hamming_map[i]);
    }

	//Now the job is ready!

//-------------------------------------------------------------------------------------

	// 3) Safely submit job in FIFO jobs queue

	
	pthread_mutex_lock(&core->job_scheduler->fifoqueue_mtx);

	//produce
	submit_job(core->job_scheduler, newjob);

	pthread_mutex_unlock(&core->job_scheduler->fifoqueue_mtx);
	sem_post(&core->job_scheduler->emptyQ); //Notify that something got into the queue


//-------------------------------------------------------------------------------------
	// sleep(1);
	
	return EC_SUCCESS;

}

///////////////////////////////////////////////////////////////////////////////////////////////

ErrorCode JobMatchDocument(Job job, DocID doc_id){
	

	//Create edit distance BK-Tree 

    Index_ptr edit_tree = malloc(sizeof(*edit_tree));

    build_entry_index(core, job->document->document->entry_list, MT_EDIT_DIST, edit_tree);
	
	// if(doc_id==3) printbk(edit_tree->root);

	//Create hamming distance BK-Trees [ one for each possible word length]
	//created an array of index trees hamming_array[num] is for the bk tree of words with num letters
	Index_ptr* hamming_array = malloc(28*sizeof(*hamming_array));
	fill_hamming_ix_array(core, hamming_array, job->document->document->entry_list, MT_HAMMING_DIST);
	
	//for every possible threshold (match_dist)
	for(int threshold=0 ; threshold<4 ; threshold++){
		
		//for every word of the edit dictionary
		for(int i=0 ; i<job->document->edit_queries->capacity ; i++){
            for(Entry edit_entry=job->document->edit_queries->array[i].entry_list->head;edit_entry!=NULL;edit_entry=edit_entry->next){
				
				EntryList results = create_entry_list();
		// pthread_mutex_lock(&core->job_scheduler->stupidmtx);
				lookup_entry_index(core,  edit_entry->word, edit_tree, threshold, results, MT_EDIT_DIST);
		// pthread_mutex_unlock(&core->job_scheduler->stupidmtx);
				// if(doc_id==3){

				// 		printf("THRESHOLD %d ENTRY WORD %s \n", threshold, edit_entry->word);
				// 		print_entrylist(results);
				// }
				//so now we have the results | the words of the document that matched with this threshold
				if(results->size!=0){
					
					// if it matched with at least one word
					//then we should add this word to the matched words of a query if:
					//  1) a query belongs to the th_box of the current threshold - th_box[threshold]
					//  2) has match type edit
					//  3) belongs to the payload of the word wwe just checked

					
					for(int j=0;j<job->document->query_edit_map[threshold]->capacity;j++){
						QueryMapNode node=&job->document->query_edit_map[threshold]->array[j];
						if(node->query_list->size>0){
							for(QueryListNode lnode=node->query_list->head;lnode!=NULL;lnode=lnode->next){
								if(check_list_existence(edit_entry->payload,lnode->query->query_id)){
									matchQuery(core, lnode->query,edit_entry->word,job->document->doc);
								}
							}
						}

					}

				}

				destroy_entry_list(results);

			}
    	} 


		//Now the same for every word of the hammign dictionary
		//for every word of the hamming dictionary
		for(int i=0 ; i<job->document->hamming_queries->capacity ; i++){
            for(Entry hamming_entry=job->document->hamming_queries->array[i].entry_list->head;hamming_entry!=NULL;hamming_entry=hamming_entry->next){
				
				EntryList results = create_entry_list();
		// pthread_mutex_lock(&core->job_scheduler->stupidmtx);
				
				lookup_entry_index(core, hamming_entry->word, hamming_array[strlen(hamming_entry->word)-4], threshold, results, MT_HAMMING_DIST);
		// pthread_mutex_unlock(&core->job_scheduler->stupidmtx);
				
				//so now we have the results | the words of the document that matched with this threshold
				if(results->size!=0){
					// if it matched with at least one word
					//then we should add this word to the matched words of a query if:
					//  1) a query belongs to the th_box of the current threshold ) th_box[threshold]
					//  2) has match type hamming
					//  3) belongs to the payload of the word wwe just checked


					for(int j=0;j<job->document->query_hamming_map[threshold]->capacity;j++){
						QueryMapNode node=&job->document->query_hamming_map[threshold]->array[j];
						if(node->query_list->size>0){
							for(QueryListNode lnode=node->query_list->head;lnode!=NULL;lnode=lnode->next){
								if(check_list_existence(hamming_entry->payload,lnode->query->query_id)){
									matchQuery(core, lnode->query,hamming_entry->word, job->document->doc);
								}
							}
						}
					}

				}

				destroy_entry_list(results);

			}
    	} 

		//And last but not least, take care of the exact queries
		//which appear only on threshold==0
		if(threshold==0){
			for(int i=0 ; i<job->document->exact_queries->capacity ; i++){
				for(Entry exact_entry=job->document->exact_queries->array[i].entry_list->head;exact_entry!=NULL;exact_entry=exact_entry->next){
					
					EntryList results = create_entry_list();
					bool result = map_find(job->document->document, exact_entry->word);
					if(result!=false){ //If there was such word in exact dictionary, there can be only one by the way
						add_entry(results, create_entry("exactresult"));
					}
					//so now we have the results | the words of the document that matched with this threshold
					if(results->size!=0){
						// if it matched with at least one word
						//then we should add this word to the matched words of a query if:
						//  1) a query belongs to the th_box of the current threshold ) th_box[threshold]
						//  2) has match type exact
						//  3) belongs to the payload of the word wwe just checked

						for(int j=0;j<job->document->query_exact_map->capacity;j++){
							QueryMapNode node=&job->document->query_exact_map->array[j];
							if(node->query_list->size>0){
								for(QueryListNode lnode=node->query_list->head;lnode!=NULL;lnode=lnode->next){
									if(check_list_existence(exact_entry->payload,lnode->query->query_id)){
										matchQuery(core, lnode->query,exact_entry->word,job->document->doc);
									}
								}
							}
						}

					}

					destroy_entry_list(results);

				}
			}
		}
	}
	// //Now that we are done with this document, we should clear all matched_words info from all queries so we do nto affect the next document
	destroy_entry_index(edit_tree);
	destroy_hamming_array(hamming_array);
	
	
	return EC_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////////////////////
int comparator (const void * p1, const void * p2)
{
  return (*(int*)p1 - *(int*)p2);
}

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
	
	//This is a tricky part
	//We want to proceed to get the results, only if:
	// -all previous matchdocument jobs have finished
	//	(so we don't grab a document's result if it hasn't been fully matched yet)

	//GetNextAvailRes will be called multiple times for every stream of documents,
	//so we sould execute the below procedure only once, using a simple flag, setting it to true when it's done for the first time
	if(core->finished_jobs!=true){
		
		//so, we will submit finished jobs for all our threads ,check job_Scduler.h for info!
		for(int i=0;i<core->job_scheduler->num_threads;i++){

			pthread_mutex_lock(&core->job_scheduler->fifoqueue_mtx);

			submit_job(core->job_scheduler, create_finish_job());

			pthread_mutex_unlock(&core->job_scheduler->fifoqueue_mtx);
			sem_post(&core->job_scheduler->emptyQ); //Notify that something got into the queue

		}

		//the job queue is a FIFO queue so the finished jobs job will be assigned after all the remaining MatchDocument jobs

		//When the threads are assigned the finished job,they will barrier wait
		//until THREADNUMBER + 1 (the main thread) are ready
		pthread_barrier_wait(&core->job_scheduler->resultsbarrier);
	
		core->finished_jobs = true;
	}

	// printf("get_next_ready\n");

	//Now we've made sure that the whole stream of documents has been matched
	
	
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
