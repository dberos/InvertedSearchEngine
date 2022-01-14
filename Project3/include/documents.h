#pragma once

#include"common_types.h"
#include"methods.h"
#include"core.h"
#include"bkt.h"


///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all query ID results associated with a dcoument
struct Document
{
	DocID doc_id;
	unsigned int num_res;
	QueryID query_ids[2000];
};


///////////////////////////////////////////////////////////////////////////////////////////////

DocumentPtr addDocument(Core core, DocID doc_id);
DocumentPtr createDoc(DocID doc_id);
void addDocumentResult(Core core, DocumentPtr doc, QueryID id);
void destroyDocument(DocumentPtr doc);
void destroyDocumentsArray(DocumentPtr* array, uint doc_number);
void clear_matchedInfo(Core core);
