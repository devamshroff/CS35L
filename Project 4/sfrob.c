#include <stdio.h>
#include <stdlib.h>


int frobcmp(const char* a, const char* b){

  for ( ; ; a++, b++){
      if(*a == ' ' && *b == ' '){
        return 0;
      }
      else if(*a == ' ' || ((*a^42) < (*b^42))){
        return -1;
      }
      else if(*b == ' ' ||  (*b ^ 42)<(*a^42)){
        return 1;
      }
    }
}

int cmpWrap(const void* a, const void* b)
{
    return frobcmp(*((const char**) a), *((const char**) b));
}

int main(void){

  
  char* word;
  int WFactor = 30;
  int WLFactor = 30;
  char** wordList;
  int wordCount = 0;
  int letterCount = 0;
  word = (char*)malloc(sizeof(char) * WFactor);
  wordList = (char**)malloc(sizeof(char*) * WLFactor);

  if( word == NULL ){
    free(word);
    free(wordList);
    fprintf(stderr, "Memory Allocation Failure");
    exit(1);
  }
  if(wordList == NULL){
    free(word);
    free(wordList);
    fprintf(stderr, "Memory Allocation Failure");
    exit(1);
  }

  char cur = '*';
  while(cur!=EOF){ 
    if(letterCount == WFactor){
      WFactor = WFactor * 2;
      word = realloc(word, sizeof(char)*WFactor);
      if(word == NULL){
	
        free(word);
        free(wordList);
        fprintf(stderr, "Memory Re-Allocation Failure");
        exit(1);
      }
    }
    cur = getchar();
    

    if(cur!=EOF){
      word[letterCount] = cur;
      letterCount++;
    }else{
      word[letterCount] = ' ';
      letterCount++;
      wordList[wordCount] = word;
      wordCount++;
    }
    
    if(cur == ' '){

	if(wordCount+1 == WLFactor){
		WLFactor = WLFactor*2;
		wordList = realloc(wordList, sizeof(char*)*WLFactor);
	if(wordList == NULL){
	  free(word);
	  free(wordList);
	  fprintf(stderr, "Memory Re-Allocation Failure");
	  exit(1);
	}
	}
      
      
      wordList[wordCount] = word;
      wordCount++;
      letterCount =0;
      word = NULL;
      WFactor = 19;
      word = malloc(sizeof(char)*WFactor);

      if(word == NULL){
        free(wordList);
        fprintf(stderr, "Memory Allocation Failure");
        exit(1);
      }
     
    } 
    }
   
  
  qsort(wordList, wordCount, sizeof(char*), cmpWrap);
  
  //switch this to putchar output and solve the realloc memory problem inside the while loop.
  
  for(int i = 0; i < wordCount; i++){
    for(int j = 0; wordList[i][j] != ' ' && wordList[i][j] != '\0'; j++){
       putchar(wordList[i][j]);
       if(wordList[i][j+1] == ' '){
	 putchar(' ');
       }
    }
  }

  for (int i =0; i<wordCount; i++){
    free(wordList[i]);
  }
  free(wordList);
  exit(0);
}
