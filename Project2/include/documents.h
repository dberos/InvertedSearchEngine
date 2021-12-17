#pragma once

#include"common_types.h"
#include"methods.h"
#include"core.h"


///////////////////////////////////////////////////////////////////////////////////////////////

// Keeps all query ID results associated with a dcoument
struct Document
{
	DocID doc_id;
	unsigned int num_res;
	QueryID* query_ids;
};


///////////////////////////////////////////////////////////////////////////////////////////////

DocumentPtr addDocument(Core core, DocumentPtr* array, DocID doc_id);
DocumentPtr createDoc(DocID doc_id);
void addDocumentResult(DocumentPtr doc, QueryID id);
void destroyDocument(DocumentPtr doc);
void destroyDocumentsArray(DocumentPtr* array, uint doc_number);
void clear_matchedInfo(Core core);
