#include"../../include/methods.h"

int deduplication(FILE* document,Map map){
    // Error checking for file
    if(document==NULL){
        fprintf(stdout,"errno= %d \n",errno);
        perror("fopen");
        return 1;
    }

    String line=NULL;
    size_t len=0;

    Dictionary dictionary=dictionary_create();
    dictionary_init(dictionary);


    while(getline(&line,&len,document)!=-1){
        // If line isn't empty
        if(strcmp(line,"\n")!=0){
            String string=strtok(line," ");
            while(string!=NULL){
                // Don't allow special characters
                if(strcmp(string,".")!=0 && strcmp(string,",")!=0){
                    // Clean-up for the word
                    String cleaner=strdup(string);
                    // Don't allow special characters at the end of the word
                    while(cleaner[strlen(cleaner)-1]=='.' || cleaner[strlen(cleaner)-1]==','
                            || cleaner[strlen(cleaner)-1]==')' || cleaner[strlen(cleaner)-1]=='\''
                                || cleaner[strlen(cleaner)-1]==']'){
                                        cleaner[strlen(cleaner)-1]='\0';
                    }
                    // First characters ' or ( to be removed
                    String cleaner1=strdup(cleaner);
                    int i=0;
                    // Multiple special characters can be together
                    // So each time jump to the correct letter, instead of cleaner+1
                    int j=1;
                    while(cleaner[i]=='\'' || cleaner[i]=='(' || cleaner[i]=='['){
                        sprintf(cleaner1,"%s",cleaner+j);
                        i++;
                        j++;
                    }

                    Word word=word_create(cleaner1);
                    // Whether first letter is capital
                    bool is_cap=dictionary_check_key(dictionary,word);
                    if(is_cap==true){
                        String str=strdup(dictionary_return_key(dictionary,word));
                        char ch=str[1];
                        String change=malloc(100);
                        String to_change=strdup(word->word);
                        // change+1 to remove the capital letter and replace it with ch
                        sprintf(change,"%c%s",ch,to_change+1);
                        String final=strdup(change);
                        if(strcmp(final,"\n")!=0){
                            map_insert(map,final);
                        }
                        free(str);
                        free(change);
                        free(to_change);
                        free(final);
                    }
                    else{
                        String final=strdup(cleaner1);
                        if(strcmp(final,"\n")!=0 && strcmp(final,"-")!=0){
                            map_insert(map,final);
                        }
                        free(final);
                    }
                    word_destroy(word);
                    free(cleaner);
                    free(cleaner1);
                    string=strtok(NULL," ");
                }
            }
        }
    }

    free(line);
    dictionary_destroy(dictionary);

    return 0;
}
