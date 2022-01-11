#include"../../include/job_scheduler.h"

extern Core core;

Thread thread_create(){
    // Allocate memory for the Thread
    Thread thread=malloc(sizeof(*thread));

    // Wait until Thread needs initialization with the routine
    thread->thread=0;
    thread->routine=NULL;

    return thread;
}

void thread_init(Thread thread,Routine routine){
    pthread_create(&thread->thread,0,(Pointer)routine,(Pointer)core);
}

void thread_destroy(Thread thread){
    // Tell the OS to release resources
    // __Maybe it will cause problems__
    // This is a test
    pthread_detach(thread->thread);

    // Wait for a thread to finish
    pthread_join(thread->thread,0);

    free(thread);
}

JobScheduler job_scheduler_create(int num_threads){
    // Allocating memory for the Job Scheduler
    JobScheduler job_scheduler=malloc(sizeof(*job_scheduler));

    // Set number of execution threads
    job_scheduler->num_threads=num_threads;
    // Allocating memory for the threads
    job_scheduler->threads=malloc(sizeof(*job_scheduler->threads)*num_threads);
    for(int i=0;i<job_scheduler->num_threads;i++){
        job_scheduler->threads[i]=thread_create();
    }

    // This is a test
    pthread_mutex_init(&job_scheduler->mutex,0);
    pthread_cond_init(&job_scheduler->cond,0);

    return job_scheduler;
}

void job_scheduler_destroy(JobScheduler job_scheduler){
    for(int i=0;i<job_scheduler->num_threads;i++){
        thread_destroy(job_scheduler->threads[i]);
    }

    // This is a test
    pthread_mutex_destroy(&job_scheduler->mutex);
    pthread_cond_destroy(&job_scheduler->cond);
    
    free(job_scheduler->threads);
    free(job_scheduler);
}

