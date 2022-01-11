#include"../../include/job_scheduler.h"

extern Core core;

void thread_init(pthread_t* thread,Routine routine){
    pthread_create(thread,0,(Pointer)routine,(Pointer)core);
}

void thread_destroy(pthread_t* thread){
    // Tell the OS to release resources
    // __Maybe it will cause problems__
    // This is a test
    pthread_detach(*thread);

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

    // This is a test
    pthread_mutex_init(&job_scheduler->mutex,0);
    pthread_cond_init(&job_scheduler->cond,0);

    return job_scheduler;
}

void job_scheduler_destroy(JobScheduler job_scheduler){
    for(int i=0;i<job_scheduler->num_threads;i++){
        thread_destroy(&job_scheduler->threads[i]);
    }

    // This is a test
    pthread_mutex_destroy(&job_scheduler->mutex);
    pthread_cond_destroy(&job_scheduler->cond);
    
    free(job_scheduler->threads);
    free(job_scheduler);
}

