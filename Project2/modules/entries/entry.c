#include"../../include/entry.h"

Entry create_entry(const String word){
    // Allocate memory for the entry
    Entry entry = malloc(sizeof(*entry));
    // Set word
    entry->word = strdup(word);
    // Create a list for the payload
    entry->payload=list_create();
    // Set next to NULL (entries also function as Entry List nodes)
    entry->next = NULL;

    return entry;
}

Entry create_entry_with_payload(const String word, List payload){
    // Allocate memory for the entry
    Entry entry = malloc(sizeof(*entry));
    // Set word
    entry->word = strdup(word);
    // Create a list for the payload
    entry->payload=list_create();
    for(ListNode temp=payload->head ; temp!=NULL ; temp=temp->next){
        list_insert_tail(entry->payload, temp->value);
    }
    // Set next to NULL (entries also function as Entry List nodes)
    entry->next = NULL;

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
    
    EntryList el = malloc(sizeof(*el));
    
    el->head = NULL;
    el->end = NULL;
    el->size = 0;

    return el;

}

unsigned int get_number_entries(const EntryList el){
    return el->size;

}

void add_entry(EntryList el, const Entry e){
    //in case this is the first entry
    if(el->head==NULL){
        el->head = e;
        el->end = e;
        // Update size
        el->size++;
        return;
    }

    //if this is not the first entry
    el->end->next = e;
    //update end
    el->end = e;
    // Update size
    el->size++;
}

Entry get_first(const EntryList el){
    if(el->head!=NULL)  return el->head;
    else return NULL;
}

Entry get_next(const EntryList el, const Entry e){

    if(e->next!=NULL) return e->next;
    else return NULL;
}

void destroy_list_nodes(Entry e,EntryList entry_list){
    if(e==NULL) return;
    destroy_list_nodes(e->next,entry_list);
    destroy_entry(e,entry_list);
}

void destroy_entry_list(EntryList el){
    destroy_list_nodes(get_first(el),el);
    free(el);
}



bool insert_entry(EntryList entry_list,String word,Pointer id){
    Entry entry=create_entry(word);
    Entry curr;
    list_insert_tail(entry->payload,id);
    if(entry_list->head!=NULL && strcmp(entry_list->head->word,word)==0){
        destroy_entry(entry,entry_list);
        // Size shouldn't be affected by decreasing it at destroy_entry
        entry_list->size++;
        list_insert_tail(entry_list->head->payload,id);
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
                list_insert_tail(curr->next->payload,id);
                return false;
            }
        }
        entry->next=curr->next;
        curr->next=entry;
    }
    entry_list->size++;
    return true;
}
