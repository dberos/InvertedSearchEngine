#include"../../include/job.h"

Job job_create(){
    // Allocating memory for the Job
    Job job=malloc(sizeof(*job));

    // Set everything NULL and call a set_job later
    job->job_type=NaN;
    job->start_query_job=NULL;
    job->end_query_job=NULL;
    job->match_document_job=NULL;
    job->get_next_avail_res_job=NULL;

    // Return the Job
    return job;
}

ErrorCode job_execute(Job job){
    switch(job->job_type){
        case STARTQUERY:
            return execute_StartQuery(job);
        case ENDQUERY:
            return execute_EndQuery(job);
        case MATCHDOCUMENT:
            return execute_MatchDocument(job);
        case GETNEXTAVAILRES:
            return execute_GetNextAvailRes(job);
        default:
            return EC_FAIL;
    }
}

void job_destroy(Job job){
    switch(job->job_type){
        case STARTQUERY:
            start_query_job_destroy(job->start_query_job);
            break;
        case ENDQUERY:
            end_query_job_destroy(job->end_query_job);
            break;
        case MATCHDOCUMENT:
            match_document_job_destroy(job->match_document_job);
            break;
        case GETNEXTAVAILRES:
            get_next_avail_res_job_destroy(job->get_next_avail_res_job);
            break;
        default:
            free(job);
            return;
    }
    free(job);
}

StartQueryJob start_query_job_create(QueryID query_id,String query_str,MatchType match_type,uint match_dist){
    // Allocating memory for the StartQuery Job
    StartQueryJob job=malloc(sizeof(*job));
    
    // Set QueryID
    job->query_id=query_id;
    // Set Query String
    job->query_str=strdup(query_str);
    // Set MatchType
    job->match_type=match_type;
    // Set Match Distance
    job->match_dist=match_dist;

    // Return the Job
    return job;
}

void set_start_query_job(Job job,StartQueryJob start_query_job){
    // Set Job's Type
    job->job_type=STARTQUERY;
    // Set Job
    job->start_query_job=start_query_job;
}

void start_query_job_destroy(StartQueryJob start_query_job){
    // Free the Query String
    free(start_query_job->query_str);
    // Free the Job
    free(start_query_job);
}

EndQueryJob end_query_job_create(QueryID query_id){
    // Allocating memory for the EndQuery Job
    EndQueryJob job=malloc(sizeof(*job));

    // Set QueryID
    job->query_id=query_id;

    // Return the Job
    return job;
}

void set_end_query_job(Job job,EndQueryJob end_query_job){
    // Set Job's Type
    job->job_type=ENDQUERY;
    // Set Job
    job->end_query_job=end_query_job;
}

void end_query_job_destroy(EndQueryJob end_query_job){
    // Free the Job
    free(end_query_job);
}

MatchDocumentJob match_document_job_create(DocID doc_id,String doc_str){
    // Allocating memory for the MatchDocument Job
    MatchDocumentJob job=malloc(sizeof(*job));

    // Set DocID
    job->doc_id=doc_id;
    // Set Document String
    job->doc_str=strdup(doc_str);

    // Return the Job
    return job;
}

void set_match_document_job(Job job, MatchDocumentJob match_document_job){
    // Set Job's Type
    job->job_type=MATCHDOCUMENT;
    // Set Job
    job->match_document_job=match_document_job;
}

void match_document_job_destroy(MatchDocumentJob match_document_job){
    // Free the Document String
    free(match_document_job->doc_str);
    // Free the Job
    free(match_document_job);
}

GetNextAvailResJob get_next_avail_res_job_create(DocID* p_doc_id,uint* p_num_res,QueryID** p_query_ids){
    // Allocating memory for the GetNextAvailRes Job
    GetNextAvailResJob job=malloc(sizeof(*job));

    // Set DocID
    job->p_doc_id=p_doc_id;
    // Set result number
    job->p_num_res=p_num_res;
    // Set result IDs
    job->p_query_ids=p_query_ids;

    // Return the Job
    return job;
}

void set_get_next_avail_res_job(Job job,GetNextAvailResJob get_next_avail_res_job){
    // Set Job's Type
    job->job_type=GETNEXTAVAILRES;
    // Set Job
    job->get_next_avail_res_job=get_next_avail_res_job;
}

void get_next_avail_res_job_destroy(GetNextAvailResJob get_next_avail_res_job){
    // Free the Job
    free(get_next_avail_res_job);
}
