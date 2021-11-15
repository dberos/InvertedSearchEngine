#include"../include/common_types.h"
#include"../include/word.h"
#include"../include/bst.h"
#include"../include/bkt.h"
#include"../include/map.h"
#include"../include/methods.h"

int main(){

    //Read the text and deduplicate
    Map map=map_create();
    FILE* doc=fopen("../data/deduplicationtext.txt","r");
    if(deduplication(doc,map)==1){
        map_destroy(map);
        return 1;
    }

    //In case we want to print the map contents
    
    // for(int i=0;i<map->capacity;i++){
    //     MapNode node=&map->array[i];
    //     if(node->bst->root!=NULL){
    //         bst_inorder(node->bst);
    //         printf("Contents number: %d \n",node->bst->size);
    //         printf("\n");
    //     }
    // }

    


    //Read queries and create Entry List
    Map query_map=map_create();
    FILE* doc2=fopen("../data/queryekf.txt","r");
    if(deduplication(doc2,query_map)==1){
        map_destroy(query_map);
        return 1;
    }
    


    //Create index BK-Tree 
    Index_ptr tree = (Index_ptr)malloc(sizeof(bkindex));
    EntryList result= create_entry_list();
    
    build_entry_index(query_map->entry_list, hamming, tree);

    //Use lookup function

    lookup_entry_index("henn", tree, 2, result, hamming);



    ///Print lookup results

    printf("\n\nLookup results:\n");

    for(Entry entry=result->head;entry!=NULL;entry=entry->next)    printf("- %s\n",entry->word);


    //Free allocated memory and exit

    destroy_entry_index(tree);
    map_destroy(map);
    map_destroy(query_map);
    destroy_entry_list(result);
    fclose(doc);
    fclose(doc2);

    return 0;
}