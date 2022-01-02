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

int dedup(String string, Core core){
    String str=strdup(string);
    
    String word=strtok(str," ");
    while(word!=NULL){
        remove_special_characters_decapitalize(word);
        map_insert(core->document,word);
        word=strtok(NULL," ");
    }

    free(str);
    return 0;
}
