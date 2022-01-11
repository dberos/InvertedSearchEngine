#pragma once

#include"common_types.h"

typedef Pointer (*Routine)(Core);

struct thread{
    pthread_t thread;   // The thread
    Routine routine;    // Its routine
};

struct job_scheduler{
    int num_threads;        // Number of execution threads
    Thread* threads;        // Execution threads
    // This is a test
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

// Creating a Thread
// Just allocating memory
Thread thread_create();

// Initialize a Thread with its routine
void thread_init(Thread thread,Routine routine);

// Destroying a Thread
void thread_destroy(Thread thread);

// Creating a Job Scheduler
JobScheduler job_scheduler_create(int num_threads);

// Destroying a Job Scheduler
void job_scheduler_destroy(JobScheduler job_scheduler);
