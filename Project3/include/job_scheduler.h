#pragma once

#include"common_types.h"


typedef Pointer (*Routine)(Core);

struct job{
    //only one of the below bool variables will be true
    //the one that is true, will define the operation  of the job that needs to be done
    bool edit_job;
    bool hamming_job;
    bool exact_job;

    //This will be true if this job is an End Job
    // if a thread is assigned an End Job then it has to stop listening to the FIFO job queue
    // for new jobs and terminate itself
    bool end_job;

    int treshold;   //and this will define the threshold of the operation
};


struct job_scheduler{
    int num_threads;        // Number of execution threads
    pthread_t* threads;     // Execution threads

    //Job FIFO Queue
    FiFoQueue q;

    //Mutexes
    pthread_mutex_t queue_consume;
    pthread_mutex_t addto_documentresults_mutex;
    pthread_mutex_t edit_mutex;
    

    // This is a test
    pthread_mutex_t mutex;
    pthread_cond_t cond;
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
Job create_job(bool edit_job, bool hamming_job, bool exact_job, int threshold);

// Create an End job
// if a thread is assigned an End Job then it has to stop listening to the FIFO job queue
// for new jobs and terminate itself
Job create_End_job();


// Destroy a job
void destroy_job(Job job);
