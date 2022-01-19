#pragma once

#include"common_types.h"
#include <pthread.h> 

typedef Pointer (*Routine)(Core);

struct job{
    //A job is a document that we want to match

    //this job should hold all the necessary doc info ( deduplicated document words, active queries of this document)
    DocInfo document;

    // if a thread is assigned an End Job then it has to stop listening to the FIFO job queue
    // for new jobs and terminate itself
    bool end_job;

    //Check GetNextAvailRes for explanation of this
    bool finish_job;

};


struct job_scheduler{
    int num_threads;        // Number of execution threads
    pthread_t* threads;     // Execution threads

    //Job FIFO Queue
    FiFoQueue q;

    //Mutexes
    pthread_mutex_t fifoqueue_mtx;
    pthread_mutex_t stupidmtx;
    
    

    //Semaphores
    sem_t emptyQ;
    
    //Barrier
    pthread_barrier_t resultsbarrier;

    
};

// Creating a Job Scheduler
JobScheduler job_scheduler_create(int num_threads);

// Destroying a Job Scheduler
void job_scheduler_destroy(JobScheduler job_scheduler);

// Initialize a Thread with its routine
void thread_init(pthread_t* thread,Routine routine);

// Destroying a Thread
void thread_destroy(pthread_t* thread);


// Recommended interfaces (ths ekfwnhshs) --------------------

// Returns the number of submitted jobs :)
int submit_job(JobScheduler sch, Job job);



// Struct Job operations -------------------------------------

// Create a job
Job create_job();

// Create an End job
// if a thread is assigned an End Job then it has to stop listening to the FIFO job queue
// for new jobs and terminate itself
Job create_End_job();


//A finish job is a job placed on the FIFO(!) job queue
//When a finish job is grabbed by a thread, then we know that all previous jobs have been submitted 
Job create_finish_job();


// Destroy a job
void destroy_job(Job job);

// Destroy an end job
void destroy_end_or_finish_job(Job job);
