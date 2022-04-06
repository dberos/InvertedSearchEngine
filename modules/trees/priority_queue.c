#include"../../include/priority_queue.h"

PriorityQueue pqueue_create(Priority priority){
    // Allocating memory for the Priority Queue
    PriorityQueue pqueue=malloc(sizeof(*pqueue));

    // Set starting size
    pqueue->size=0;
    // Set starting capacity
    pqueue->capacity=10;
    // Set Priority
    pqueue->priority=priority;

    // Allocate memory for the array
    pqueue->array=calloc(pqueue->capacity,sizeof(*pqueue->array));

    // Return the Priority Queue
    return pqueue;
}

void pqueue_destroy(PriorityQueue pqueue){
    // Free the array
    free(pqueue->array);
    // Free the Priority Queue
    free(pqueue);
}

PriorityQueueNode pqueue_at(PriorityQueue pqueue,int pos){
    // Return the Node if it exists
    return pos-1<pqueue->size && pos-1>=0 ? &pqueue->array[pos-1] : NULL;
}

QueryID pqueue_get(PriorityQueue pqueue,int pos){
    // Return the id of the Node, if Node exists
    return pqueue_at(pqueue,pos)!=NULL ? pqueue_at(pqueue,pos)->id : 0;
}

void pqueue_set(PriorityQueue pqueue,QueryID id,int pos){
    // Find the Node
    PriorityQueueNode node=pqueue_at(pqueue,pos);
    // Set the id of the Node if Node exists
    node->id=(node!=NULL) ? id : 0;
}

void pqueue_swap(PriorityQueue pqueue,int pos1,int pos2){
    // Find id of position 1
    QueryID id1=pqueue_get(pqueue,pos1);
    // Find id of position 2
    QueryID id2=pqueue_get(pqueue,pos2);

    // Set id of position 1
    pqueue_set(pqueue,id2,pos1);
    // Set id of position 2
    pqueue_set(pqueue,id1,pos2);
}

void pqueue_bubble_up(PriorityQueue pqueue,int pos){
    if(pos==1){
        // Found root
        return;
    }

    // Find parent position
    int parent=PARENT(pos);

    // Check for Priority and get the result
    Priority priority=pqueue->priority;
    int res=0;
    switch(priority){
        case MAX:
            res=pqueue_get(pqueue,parent)-pqueue_get(pqueue,pos);
            break;
        case MIN:
            res=pqueue_get(pqueue,pos)-pqueue_get(pqueue,parent);
            break;
    }
    
    // Fix Property if needed
    if(res<0){
        pqueue_swap(pqueue,pos,parent);
        pqueue_bubble_up(pqueue,parent);
    }
}

void pqueue_bubble_down(PriorityQueue pqueue,int pos){
    // Find left
    int left=LEFT(pos);
    // Find right
    int right=RIGHT(pos);
    // Find last
    int last=pqueue->size;

    if(left>last){
        return;
    }

    // MIN / MAX Priority
    int node=left;

    // Check for Priority and get the result
    Priority priority=pqueue->priority;
    int res=0;
    switch(priority){
        case MAX:
            res=pqueue_get(pqueue,left)-pqueue_get(pqueue,right);
            break;
        case MIN:
            res=pqueue_get(pqueue,right)-pqueue_get(pqueue,left);
            break;
    }
    
    // Find MIN / MAX
    if(right<=last && res<0){
        node=right;
    }

    switch(priority){
        case MAX:
            res=pqueue_get(pqueue,pos)-pqueue_get(pqueue,node);
            break;
        case MIN:
            res=pqueue_get(pqueue,node)-pqueue_get(pqueue,pos);
            break;
    }

    // Fix Property if needed
    if(res<0){
        pqueue_swap(pqueue,node,pos);
        pqueue_bubble_down(pqueue,node);
    }
}

void pqueue_insert(PriorityQueue pqueue,QueryID id){
    // Increase the size
    pqueue->size++;

    // Check whether array needs realloc
    if(pqueue->capacity<pqueue->size){
        // Double up the capacity
        pqueue->capacity*=2;
        // Realloc
        pqueue->array=realloc(pqueue->array,sizeof(*pqueue->array)*pqueue->capacity);
    }

    // Position for the insertion since size is increased
    int pos=pqueue->size-1;
    // Node for the insertion
    PriorityQueueNode node=&pqueue->array[pos];
    // Set id
    node->id=id;

    // Fix property
    pqueue_bubble_up(pqueue,pqueue->size);
}

void pqueue_remove(PriorityQueue pqueue){
    // Find last node
    int last=pqueue->size;
    // Swap with root
    pqueue_swap(pqueue,1,last);

    // Position for the removal
    int pos=pqueue->size-1;
    // Node for the removal
    PriorityQueueNode node=&pqueue->array[pos];

    // Set new id
    node->id=0;
    // Decrease size
    pqueue->size--;

    // Check whether array needs realloc
    if(pqueue->capacity>pqueue->size*4){
        pqueue->capacity/=2;
        pqueue->array=realloc(pqueue->array,sizeof(*pqueue->array)*pqueue->capacity);
    }

    // Fix property
    pqueue_bubble_down(pqueue,1);
}

QueryID pqueue_find(PriorityQueue pqueue){
    // Return MIN / MAX
    return pqueue_get(pqueue,1);
}
