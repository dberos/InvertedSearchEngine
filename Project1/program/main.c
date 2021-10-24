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
        if(map->array[i].state==occupied){
            printf("NODE: %s \n",map->array[i].key);
            bst_inorder(map->array[i].bst);
            printf("\n");
        }
    }
    map_destroy(map);
    fclose(doc);

    return 0;
}