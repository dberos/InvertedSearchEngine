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

    // Create an alphabet to insert at a Map so i can convert capital letters
    String words[]={"Aa","Bb","Cc","Dd","Ee","Ff","Gg","Hh","Ii","Jj",
                        "Kk","Ll","Mm","Nn","Oo","Pp","Qq","Rr","Ss","Tt",
                            "Uu","Vv","Ww","Xx","Yy","Zz"};


    // Create the Strings to Words
    Word* capitals=malloc(sizeof(*capitals)*26);
    for(int i=0;i<26;i++){
        capitals[i]=word_create(words[i][0],words[i]);
    }

    // Map for the alphabet
    Map caps_map=map_create();
    for(int i=0;i<26;i++){
        map_insert(caps_map,capitals[i]);
    }


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

                    Word word=word_create(cleaner1[0],cleaner1);
                    // Whether first letter is capital
                    bool is_cap=map_check_key(caps_map,word);
                    if(is_cap==true){
                        String str=strdup(map_return_key(caps_map,word));
                        char ch=str[1];
                        String change=malloc(100);
                        String to_change=strdup(word->word);
                        // change+1 to remove the capital letter and replace it with ch
                        sprintf(change,"%c%s",ch,to_change+1);
                        String final=strdup(change);
                        Word word1=word_create(final[0],final);
                        if(strcmp(word1->word,"\n")!=0){
                            map_insert(map,word1);
                        }
                        word_destroy(word1);
                        free(str);
                        free(change);
                        free(to_change);
                        free(final);
                    }
                    else{
                        String final=strdup(cleaner1);
                        Word word1=word_create(final[0],final);
                        if(strcmp(word1->word,"\n")!=0 && strcmp(word1->word,"-")!=0){
                            map_insert(map,word1);
                        }
                        word_destroy(word1);
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

    for(int i=0;i<26;i++){
        word_destroy(capitals[i]);
    }
    free(capitals);
    map_destroy(caps_map);

    return 0;
}
