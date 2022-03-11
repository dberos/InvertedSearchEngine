#include"../../include/entry.h"

Entry create_entry(String word){
    // Allocate memory for the entry
    Entry entry=malloc(sizeof(*entry));
    // Set word
    entry->word=strdup(word);
    // Create a list for the payload
    entry->payload=list_create();
    // Set next to NULL
    entry->next=NULL;
    // Return the Entry
    return entry;
}

void destroy_entry(Entry entry,EntryList entry_list){
    // Free the word
    free(entry->word);
    // Destroy the list for the payload
    list_destroy(entry->payload);
    // Free the entry
    free(entry);
    // Decrease the size of the entry list
    entry_list->size--;
}

EntryList create_entry_list(){
    // Allocating memory for the Entry List
    EntryList entry_list=malloc(sizeof(*entry_list));
    // Set head to NULL
    entry_list->head=NULL;
    // Set tail to NULL
    entry_list->tail=NULL;
    // Set starting size
    entry_list->size=0;
    // Return the List
    return entry_list;

}

uint get_number_entries(const EntryList el){
   return el->size;
}

void add_entry(EntryList entry_list,Entry entry){
    // If list is empty
    if(entry_list->head==NULL){
        entry_list->head=entry;
        entry_list->tail=entry;
    }
    else{
        entry_list->tail->next=entry;
        entry_list->tail=entry;
    }

    entry_list->size++;
}

Entry get_first(const EntryList entry_list){
    return entry_list->head!=NULL ? entry_list->head : NULL;
}

Entry get_next(EntryList entry_list,Entry entry){
    return entry->next!=NULL ? entry->next : NULL;
}

void destroy_entry_list(EntryList entry_list){
    Entry prev;
    Entry curr;
    curr=entry_list->head;
    while(curr!=NULL){
        prev=curr;
        curr=curr->next;
        if(prev!=NULL){
            destroy_entry(prev,entry_list);
        }
    }
    free(entry_list);
}

bool insert_entry(EntryList entry_list,String word,uint id){
    Entry entry=create_entry(word);
    Entry curr;
    list_insert_head(entry->payload,id);
    if(entry_list->head!=NULL && strcmp(entry_list->head->word,word)==0){
        destroy_entry(entry,entry_list);
        // Size shouldn't be affected by decreasing it at destroy_entry
        entry_list->size++;
        list_insert_head(entry_list->head->payload,id);
        return false;
    }
    if(entry_list->head==NULL){
        entry->next=entry_list->head;
        entry_list->head=entry;
    }
    else{
        curr=entry_list->head;
        while(curr->next!=NULL){
            if(strcmp(curr->next->word,word)!=0){
                curr=curr->next;
            }
            else{
                destroy_entry(entry,entry_list);
                // Size shouldn't be affected by decreasing it at destroy_entry
                entry_list->size++;
                list_insert_head(curr->next->payload,id);
                return false;
            }
        }
        entry->next=curr->next;
        curr->next=entry;
    }
    entry_list->size++;
    return true;
}
