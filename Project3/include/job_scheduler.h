#pragma once

#include"common_types.h"

typedef Pointer (*Routine)(Core);

struct job_scheduler{
    int num_threads;        // Number of execution threads
    pthread_t* threads;     // Execution threads
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
