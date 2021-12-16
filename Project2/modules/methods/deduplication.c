#include"../../include/methods.h"

void remove_special_characters_decapitalize(String word){
    
    String temp, finalstring;

    temp = word;
    finalstring = word;

    for(; *temp!= '\0' ; temp++){
        *finalstring = *temp;

        if ((*finalstring >= '0' && *finalstring <= '9') || (*finalstring >= 'A' && *finalstring <= 'Z') || (*finalstring >= 'a' && *finalstring <= 'z')){
            *finalstring = tolower(*finalstring); //decapitalize
            finalstring++;
        }

    }

    *finalstring = '\0';

}


int deduplication(FILE* document,Map map){
    // Error checking for file
    if(document==NULL){
        fprintf(stdout,"errno= %d \n",errno);
        perror("fopen");
        return 1;
    }

    String line=NULL;
    size_t len=0;

    while(getline(&line,&len,document)!=-1){
        
        //if line isn't empty
        if(strcmp(line, "\n")){
            //tokenize line
            String word = strtok(line, " ");
            while(word!=NULL){
                //remove special characters & decapitalize @ utilities.h
                remove_special_characters_decapitalize(word);
                
                if(!strcmp(word, "\n") || !strcmp(word, "")){
                    word=strtok(NULL," ");
                    continue;
                }

                map_insert(map, word);
                
                word=strtok(NULL," ");
            }
        
        }
    }


    free(line);

    return 0;
}

int dedup(String string, Core core, QueryID id, MatchType match_type){
    String str=strdup(string);
    if(id>0){ //if this is about a query deduplication
        String word=strtok(str," ");
        while(word!=NULL){
            remove_special_characters_decapitalize(word);
            //check methods.h for the core struct
            if(match_type==MT_EXACT_MATCH) dictionary_insert(core->exact_queries, word,&id);
            else if(match_type==MT_EDIT_DIST) dictionary_insert(core->edit_queries, word,&id);
            else dictionary_insert(core->hamming_queries, word,&id);
            word=strtok(NULL," ");
        }
    }
    else{ //if this is a document deduplication
        String word=strtok(str," ");
        while(word!=NULL){
            remove_special_characters_decapitalize(word);
            map_insert_doc(core->document,word);
            word=strtok(NULL," ");
        }
    }
    free(str);
    return 0;
}
