#pragma once

#include"common_types.h"
#include"core.h"
#include"methods.h"
#include"queue.h"
#include"document_queue.h"

typedef Pointer (*Routine)(Core);

struct job_scheduler{
    Queue query_queue;
    Queue match_queue;
    Queue res_queue;
    DocumentQueue document_queue;

    atomic_int fin;
    int num_query_threads;
    int num_match_threads;
    int num_res_threads;
    pthread_t* query_threads;
    pthread_t* match_threads;
    pthread_t* res_threads;

    pthread_mutex_t main_mutex;
    pthread_mutex_t query_queue_mutex;
    pthread_cond_t query_queue_cond;
    pthread_barrier_t match_barrier;
    sem_t query_queue_semaphore;
    pthread_mutex_t add_query_job_mutex;

    pthread_mutex_t match_queue_mutex;
    pthread_cond_t match_queue_cond;
    pthread_mutex_t add_document_mutex;
    pthread_mutex_t bkt_mutex;
    pthread_mutex_t dictionary_mutex;
    pthread_mutex_t payload_mutex;
    pthread_mutex_t post_mutex;
    sem_t semaphore;
    pthread_mutex_t query_vector_mutex;
    pthread_mutex_t dictionary_mutex2;
    pthread_mutex_t barrier_mutex;

    pthread_mutex_t res_queue_mutex;
    pthread_cond_t res_queue_cond;
    sem_t res_semaphore;
    pthread_mutex_t add_res_job_mutex;
    pthread_barrier_t res_barrier;
    pthread_mutex_t add_core_document_mutex;
    atomic_int total_jobs;
    atomic_int finished_jobs;
    pthread_mutex_t jobs_mutex;
};

// Creating a Job Scheduler
JobScheduler job_scheduler_create(int num_query_threads,int num_match_threads,int num_res_threads);

// Destroying a Job Scheduler
void job_scheduler_destroy(JobScheduler job_scheduler);

// Starting a thread
void thread_init(pthread_t* thread,Routine routine);

// Destroying a thread
void thread_destroy(pthread_t* thread);

// Thread Query routine
Routine query_routine(Core core);

// Thread MatchDocument routine
Routine match_routine(Core core);

// Thread GetNextAvailRes routine
Routine res_routine(Core core);
