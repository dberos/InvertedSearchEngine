#include"../../include/job_scheduler.h"

extern Core core;
extern JobScheduler job_scheduler;

JobScheduler job_scheduler_create(int num_query_threads,int num_match_threads,int num_res_threads){
    // Assert at least 1 thread of each type exists
    assert(num_query_threads>0 && num_match_threads>0 && num_res_threads>0);

    // Allocating memory for the Job Scheduler
    JobScheduler job_scheduler=malloc(sizeof(*job_scheduler));
    
    // Creating a Queue for Query Jobs
    job_scheduler->query_queue=queue_create();
    // Creating a Queue for MatchDocument
    job_scheduler->match_queue=queue_create();
    // Creating a Queue for GetNextAvailRes
    job_scheduler->res_queue=queue_create();
    // Creating a Queue to get available documents for GetNextAvailRes
    job_scheduler->document_queue=document_queue_create();
    // Set number of Query threads
    job_scheduler->num_query_threads=num_query_threads;
    // Set number of MatchDocument threads
    job_scheduler->num_match_threads=num_match_threads;
    // Set number of GetNextAvailRes threads
    job_scheduler->num_res_threads=num_res_threads;
    // Allocating memory for the Query threads
    job_scheduler->query_threads=malloc(sizeof(*job_scheduler->query_threads)*job_scheduler->num_query_threads);
    // Allocating memory for the MatchDocument threads
    job_scheduler->match_threads=malloc(sizeof(*job_scheduler->match_threads)*num_match_threads);
    // Allocating memory for the GetNextAvailRes threads
    job_scheduler->res_threads=malloc(sizeof(*job_scheduler->res_threads)*job_scheduler->num_res_threads);

    // Creating a mutex for the main thread
    pthread_mutex_init(&job_scheduler->main_mutex,0);
    // Creating the mutex for the Query Queue
    pthread_mutex_init(&job_scheduler->query_queue_mutex,0);
    // Creating the condition variable for the Query Queue
    pthread_cond_init(&job_scheduler->query_queue_cond,0);
    // Creating a barrier for MatchDocument to procceed
    pthread_barrier_init(&job_scheduler->match_barrier,0,2);
    // Creating a semaphore for Query Queue Job grabs
    sem_init(&job_scheduler->query_queue_semaphore,0,0);
    // Creating a mutex for safe Query Queue Job submissions
    pthread_mutex_init(&job_scheduler->add_query_job_mutex,0);

    // Creating a mutex to safely get a Job from the Match Queue
    pthread_mutex_init(&job_scheduler->match_queue_mutex,0);
    // Creating a condition variable for the Match Queue waiting
    pthread_cond_init(&job_scheduler->match_queue_cond,0);
    // Creating the mutex to add a new Document for processing
    pthread_mutex_init(&job_scheduler->add_document_mutex,0);
    // Creating the mutex for the BK-Trees
    pthread_mutex_init(&job_scheduler->bkt_mutex,0);
    // Creating the mutex for the Dictionaries
    pthread_mutex_init(&job_scheduler->dictionary_mutex,0);
    // Creating the mutex for rest Dictionary operations for no deadlocks
    pthread_mutex_init(&job_scheduler->dictionary_mutex2,0);
    // Creating the mutex for Payloads
    pthread_mutex_init(&job_scheduler->payload_mutex,0);
    // Creating the mutex to post at main thread
    pthread_mutex_init(&job_scheduler->post_mutex,0);
    // Creating a semaphore to start GetNextAvailRes
    sem_init(&job_scheduler->semaphore,0,0);
    // Creating the mutex for the active Query Set
    pthread_mutex_init(&job_scheduler->query_vector_mutex,0);
    // Creating the mutex for the barrier
    pthread_mutex_init(&job_scheduler->barrier_mutex,0);

    // Creating the mutex for the GetNextAvailRes Queue
    pthread_mutex_init(&job_scheduler->res_queue_mutex,0);
    // Creating the condition variable for the GetNextAvailRes Queue
    pthread_cond_init(&job_scheduler->res_queue_cond,0);
    // Creating the semaphore for the GetNextAvailRes Queue
    sem_init(&job_scheduler->res_semaphore,0,0);
    // Creating the mutex to add new GetNextAvailRes at the Queue
    pthread_mutex_init(&job_scheduler->add_res_job_mutex,0);
    // Creating the barrier for the results
    pthread_barrier_init(&job_scheduler->res_barrier,0,2);
    // Creating the mutex to insert Document at the main Queue for the destroys
    pthread_mutex_init(&job_scheduler->add_core_document_mutex,0);
    // Creating the mutex for the Jobs count
    pthread_mutex_init(&job_scheduler->jobs_mutex,0);
    // Set starting number of Jobs
    job_scheduler->total_jobs=0;
    // Set starting number of finished Jobs
    job_scheduler->finished_jobs=0;

    // Set program finishing flag to 0
    job_scheduler->fin=0;
    
    // Return the Job Scheduler
    return job_scheduler;
}

void job_scheduler_destroy(JobScheduler job_scheduler){
    // Destroying the mutex for the Query Queue
    pthread_mutex_destroy(&job_scheduler->query_queue_mutex);
    // Destroying the condition variable for the Query Queue
    pthread_cond_destroy(&job_scheduler->query_queue_cond);
    // Destroying the barrier for each MatchDocument call
    pthread_barrier_destroy(&job_scheduler->match_barrier);
    // Destroying the semaphore for the Query Queue Job grabs
    sem_destroy(&job_scheduler->query_queue_semaphore);
    // Destroying the mutex for safe Query Job submissions
    pthread_mutex_destroy(&job_scheduler->add_query_job_mutex);

    // Destroying the mutex for the Match Queue
    pthread_mutex_destroy(&job_scheduler->match_queue_mutex);
    // Destroying the condition variable for the Match Queue
    pthread_cond_destroy(&job_scheduler->match_queue_cond);
    // Destroying the mutex for the main thread
    pthread_mutex_destroy(&job_scheduler->main_mutex);
    // Destroying the mutex for Document processing
    pthread_mutex_destroy(&job_scheduler->add_document_mutex);
    // Destroying the mutex for the BK-Trees
    pthread_mutex_destroy(&job_scheduler->bkt_mutex);
    // Destroying the mutex for the Dictionaries
    pthread_mutex_destroy(&job_scheduler->dictionary_mutex);
    // Destroying the second Dictionary mutex
    pthread_mutex_destroy(&job_scheduler->dictionary_mutex2);
    // Destroying the mutex for Payloads
    pthread_mutex_destroy(&job_scheduler->payload_mutex);
    // Destroying the mutex to post at main thread
    pthread_mutex_destroy(&job_scheduler->post_mutex);
    // Destroying the semaphore for GetNextAvailRes
    sem_destroy(&job_scheduler->semaphore);
    // Destroying the mutex for the active Query Set
    pthread_mutex_destroy(&job_scheduler->query_vector_mutex);
    // Destroying the mutex for the barrier
    pthread_mutex_destroy(&job_scheduler->barrier_mutex);
    

    // Destroying the mutex for the GetNextAvailRes Queue
    pthread_mutex_destroy(&job_scheduler->res_queue_mutex);
    // Destroying the condition variable for the GetNextAvailRes Queue
    pthread_cond_destroy(&job_scheduler->res_queue_cond);
    // Destroying the semaphore for the GetNextAvailRes Queue
    sem_destroy(&job_scheduler->res_semaphore);
    // Destroying the mutex for GetNextAvailRes Jobs
    pthread_mutex_destroy(&job_scheduler->add_res_job_mutex);
    // Destroying the barrier for the results
    pthread_barrier_destroy(&job_scheduler->res_barrier);
    // Destroying the mutex for the main Queue
    pthread_mutex_destroy(&job_scheduler->add_core_document_mutex);
    // Destroying the mutex for the Jobs count
    pthread_mutex_destroy(&job_scheduler->jobs_mutex);
    

    // Destroying the Query Queue
    queue_destroy(job_scheduler->query_queue);
    // Destroying the MatchDocument Queue
    queue_destroy(job_scheduler->match_queue);
    // Destroying the GetNextAvailRes Queue
    queue_destroy(job_scheduler->res_queue);
    // Destroying the Queue for available documents
    document_queue_destroy(job_scheduler->document_queue);
    // Free the allocated memory for the Query threads
    free(job_scheduler->query_threads);
    // Free the allocated memory for the Match threads
    free(job_scheduler->match_threads);
    // Free the allocated memory for the GetNextAvailRes threads
    free(job_scheduler->res_threads);
    // Free the Job Scheduler
    free(job_scheduler);
}

void thread_init(pthread_t* thread,Routine routine){
    pthread_create(thread,0,(Pointer)routine,(Pointer)core);
}

void thread_destroy(pthread_t* thread){
    // Join the thread
    pthread_join(*thread,0);
    /**
     * Detach the thread
     * Tell to OS to release resources
     * Without it, sometimes ended in deadlock
     * After many make run | valgrind in a row without make clean
     **/
    pthread_detach(*thread);
}

Routine query_routine(Core core){
    while(job_scheduler->fin==0){
        // Wait at semaphore 
        sem_wait(&job_scheduler->query_queue_semaphore);
        // Check whether program is finishing
        if(job_scheduler->fin==1){
            return 0;
        }
        // Lock the mutex to safely grab a Job
        pthread_mutex_lock(&job_scheduler->add_query_job_mutex);
        // Acquire a Job from the Queue
        Job job=queue_remove_first(job_scheduler->query_queue);
        // Unlock the mutex
        pthread_mutex_unlock(&job_scheduler->add_query_job_mutex);
        
        // Execute the Job
        if(job!=NULL){
            job_execute(job);
        }
        else{
            // Lock the mutex
            pthread_mutex_lock(&job_scheduler->res_queue_mutex);
            // Wait for signal
            while(job_scheduler->total_jobs!=job_scheduler->finished_jobs){
                pthread_cond_wait(&job_scheduler->res_queue_cond,&job_scheduler->res_queue_mutex);
            }
            // Wait at barrier for MatchDocument to procceed
            pthread_barrier_wait(&job_scheduler->match_barrier);
            // Unlock the mutex
            pthread_mutex_unlock(&job_scheduler->res_queue_mutex);
        }
    }
    return 0;
}

Routine match_routine(Core core){
    while(job_scheduler->fin==0){
        // Lock the mutex for the Queue
        pthread_mutex_lock(&job_scheduler->match_queue_mutex);
        // Check whether Queue is empty
        while(job_scheduler->match_queue->size<=0){
            // Check whether program is finishing
            if(job_scheduler->fin==1){
                // Acquire a Job from the Queue
                Job job=queue_remove_first(job_scheduler->match_queue);
                // Unlock the mutex
                pthread_mutex_unlock(&job_scheduler->match_queue_mutex);
                // Execute the Job
                if(job!=NULL){
                    job_execute(job);
                }
                return 0;
            }
            // Wait at condition variable
            pthread_cond_wait(&job_scheduler->match_queue_cond,&job_scheduler->match_queue_mutex);
            // Check whether program is finishing
            if(job_scheduler->fin==1){
                // Acquire a Job from the Queue
                Job job=queue_remove_first(job_scheduler->match_queue);
                // Unlock the mutex
                pthread_mutex_unlock(&job_scheduler->match_queue_mutex);
                // Execute the Job
                if(job!=NULL){
                    job_execute(job);
                }
                return 0;
            }
        }
        // Acquire a Job from the Queue
        Job job=queue_remove_first(job_scheduler->match_queue);
        // Unlock the mutex
        pthread_mutex_unlock(&job_scheduler->match_queue_mutex);
        // Execute the Job
        if(job!=NULL){
            job_execute(job);
        }
    }
    return 0;
}

Routine res_routine(Core core){
    while(job_scheduler->fin==0){
        // Wait at semaphore
        sem_wait(&job_scheduler->res_semaphore);
        // Check whether program is finishing
        if(job_scheduler->fin==1){
            return 0;
        }
        // Lock the mutex to safely grab a Job
        pthread_mutex_lock(&job_scheduler->add_res_job_mutex);
        // Obtain a Job
        Job job=queue_remove_first(job_scheduler->res_queue);
        // Unlock the mutex
        pthread_mutex_unlock(&job_scheduler->add_res_job_mutex);
        // Execute the Job
        if(job!=NULL){
            job_execute(job);
        }
    }
    return 0;
}
