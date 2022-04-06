#pragma once

#include"common_types.h"
#include"core.h"
#include"methods.h"
#include"job_scheduler.h"
#include"bk-tree.h"
#include"document.h"

struct job{
    JobType job_type;
    StartQueryJob start_query_job;
    EndQueryJob end_query_job;
    MatchDocumentJob match_document_job;
    GetNextAvailResJob get_next_avail_res_job;
};

struct start_query_job{
    QueryID query_id;
    String query_str;
    MatchType match_type;
    uint match_dist;
};

struct end_query_job{
    QueryID query_id;
};

struct match_document_job{
    DocID doc_id;
    String doc_str;
};

struct get_next_avail_res_job{
    DocID* p_doc_id;
    uint* p_num_res;
    QueryID** p_query_ids;
};

// Creating a Job
Job job_create();

// Executing a Job
ErrorCode job_execute(Job job);

// Destroying a Job
void job_destroy(Job job);

// Creating a StartQuery Job
StartQueryJob start_query_job_create(QueryID query_id,String query_str,MatchType match_type,uint match_dist);

// Setting Job's type as a StartQuery Job
void set_start_query_job(Job job,StartQueryJob start_query_job);

// Execute a StartQuery Job
ErrorCode execute_StartQuery(Job job);

// Destroying a StartQuery Job
void start_query_job_destroy(StartQueryJob start_query_job);

// Creating an EndQuery Job
EndQueryJob end_query_job_create(QueryID query_id);

// Setting Job's type as an EndQuery Job
void set_end_query_job(Job job,EndQueryJob end_query_job);

// Execute an EndQuery Job
ErrorCode execute_EndQuery(Job job);

// Destroying an EndQuery Job
void end_query_job_destroy(EndQueryJob end_query_job);

// Creating a MatchDocument Job
MatchDocumentJob match_document_job_create(DocID doc_id,String doc_str);

// Setting Job type as a MatchDocument Job
void set_match_document_job(Job job, MatchDocumentJob match_document_job);

// Execute a MatchDocument Job
ErrorCode execute_MatchDocument(Job job);

// Destroying a MatchDocumentJob
void match_document_job_destroy(MatchDocumentJob match_document_job);

// Creating a GetNextAvailRes Job
GetNextAvailResJob get_next_avail_res_job_create(DocID* p_doc_id,uint* p_num_res,QueryID** p_query_ids);

// Setting a Job's type as GetNextAvailRes Job
void set_get_next_avail_res_job(Job job,GetNextAvailResJob get_next_avail_res_job);

// Execute a GetNextAvailRes Job
ErrorCode execute_GetNextAvailRes(Job job);

// Destroying a GetNextAvailRes Job
void get_next_avail_res_job_destroy(GetNextAvailResJob get_next_avail_res_job);
