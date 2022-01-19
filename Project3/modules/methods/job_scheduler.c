#include"../../include/job_scheduler.h"
#include"../../include/fifoqueue.h"
#include"../../include/methods.h"



extern Core core;

void thread_init(pthread_t* thread,Routine routine){
    pthread_create(thread,0,(Pointer)routine,(Pointer)core);
}

void thread_destroy(pthread_t* thread){
    // Tell the OS to release resources
    // __Maybe it will cause problems__
    // This is a test
    // pthread_detach(*thread);

    // Wait for a thread to finish
    pthread_join(*thread,0);
}

JobScheduler job_scheduler_create(int num_threads){
    // Allocating memory for the Job Scheduler
    JobScheduler job_scheduler=malloc(sizeof(*job_scheduler));

    // Set number of execution threads
    job_scheduler->num_threads=num_threads;
    // Allocating memory for the threads
    job_scheduler->threads=malloc(sizeof(*job_scheduler->threads)*num_threads);

    //Create the FIFO Queue that will hold our Jobs
    job_scheduler->q = fifoqueue_create();

    //Initialize mutexes
    pthread_mutex_init(&job_scheduler->fifoqueue_mtx,NULL);
    pthread_mutex_init(&job_scheduler->stupidmtx,NULL);
    
    
    //Initialize semaphores
    sem_init(&job_scheduler->emptyQ, 0, 0);

    //Initialize barriers
    pthread_barrier_init(&job_scheduler->resultsbarrier, NULL, THREADS_NUMBER+1);

    return job_scheduler;
}

void job_scheduler_destroy(JobScheduler job_scheduler){
    

    //Destroy job fifo queue
    fifoqueue_destroy(job_scheduler->q);

    //Destroy mutexes
    pthread_mutex_destroy(&job_scheduler->fifoqueue_mtx);
    pthread_mutex_destroy(&job_scheduler->stupidmtx);
    
    

    //Destroy semaphores
    sem_destroy(&job_scheduler->emptyQ);
    
    //Destroy barriers
    pthread_barrier_destroy(&job_scheduler->resultsbarrier);

    free(job_scheduler->threads);
    free(job_scheduler);
}


// Recommended interfaces (ths ekfwnhshs) --------------------

int submit_job(JobScheduler sch, Job job){ //returns the number of submitted jobs :)
    fifoqueue_insert(sch->q, job);
    return sch->q->size;
}





// Struct Job operations -------------------------------------


Job create_job(){
    
    Job newjob = (Job)malloc(sizeof(struct job));
    
    newjob->document = create_docinfo();

    newjob->end_job = false;

    newjob->finish_job = false;

    return newjob;

}

// if a thread is assigned an End Job then it has to stop listening to the FIFO job queue
// for new jobs and terminate itself
Job create_End_job(){
    Job newjob = (Job)malloc(sizeof(struct job));

    newjob->document = NULL;

    newjob->end_job = true;

    newjob->finish_job = false;

    return newjob;
}

//check job_Scheduler.h for explanation
Job create_finish_job(){
    Job newjob = (Job)malloc(sizeof(struct job));

    newjob->document = NULL;

    newjob->end_job = false;

    newjob->finish_job = true;

    return newjob;
}


void destroy_end_or_finish_job(Job job){
    free(job);
}

void destroy_job(Job job){

    map_destroy(job->document->document);
    dictionary_destroy(job->document->edit_queries);
    dictionary_destroy(job->document->hamming_queries);
    dictionary_destroy(job->document->exact_queries);
    query_map_destroy(job->document->query_exact_map);
    for(int i=0;i<4;i++){
        query_map_destroy(job->document->query_edit_map[i]);
        query_map_destroy(job->document->query_hamming_map[i]);
    }
    free(job->document->query_edit_map);
    free(job->document->query_hamming_map);
    // destroy_entry_index(job->document->edit_tree);
	// destroy_hamming_array(job->document->hamming_array);
    free(job->document);
    free(job);
}

