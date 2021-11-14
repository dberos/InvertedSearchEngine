#include"../include/common_types.h"
#include"../include/word.h"
#include"../include/bst.h"
#include"../include/map.h"
#include"../include/methods.h"

int main(){
    Map map=map_create();
    FILE* doc=fopen("../data/deduplication.txt","r");
    if(deduplication(doc,map)==1){
        map_destroy(map);
        return 1;
    }
    for(int i=0;i<map->capacity;i++){
        MapNode node=&map->array[i];
        if(node->bst->root!=NULL){
            bst_inorder(node->bst);
            printf("SIZE %d \n",node->bst->size);
            printf("\n");
        }
    }
    int size=0;
    for(Entry entry=map->entry_list->head;entry!=NULL;entry=entry->next){
        printf("ENTRY: %s \n",entry->word);
        size++;
    }
    printf("MAP SIZE %d \n",map->size);
    printf("ENTRY LIST SIZE: %d \n",size);
    map_destroy(map);
    fclose(doc);

    return 0;
}