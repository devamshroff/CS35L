#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

int option_f = 0;
unsigned char decode(char a){
   a = a ^ 42;
   if(option_f){
    a = toupper((unsigned char)a);
  }
  return a;
}
  

int frobcmp(const char* a, const char* b){

  for ( ; ; a++, b++){
      if(*a == ' ' && *b == ' '){
        return 0;
      }
      else if(*a == ' ' || (decode(*a) < decode(*b))){
        return -1;
      }
      else if(*b == ' ' ||  decode(*b)<decode(*a)){
        return 1;
      }
    }
}

int cmpWrap(const void* a, const void* b)
{
    return frobcmp(*((const char**) a), *((const char**) b));
}

void readError(ssize_t state)
{
  if(state < 0)
    {
      fprintf(stderr, "Error while reading file!");
      exit(1);
    }
}

int main(int argc, char* argv[]){
  if (argc == 3 || argc == 2){
    if (!(strncmp(argv[1], "-f", 2))){  
      option_f = 1;
    }
  }
  //means that fstat does not work
  struct stat fstat_obj;
  if(fstat(0,&fstat_obj)<0){
    fprintf(stderr, "Error: fstat doesn't work");
    exit(1);
  }

  char* word;
  int WFactor = 30;
  int WLFactor = 30;
  char** wordList;
  int wordCount = 0;
  int letterCount = 0;
  char* bigWordList;
  word = (char*)malloc(sizeof(char) * WFactor);
  // wordList = (char**)malloc(sizeof(char*) * WLFactor);

  if( word == NULL ){
    free(word);
    free(wordList);
    fprintf(stderr, "Memory Allocation Failure");
    exit(1);
  }
  
  /* if(wordList == NULL){
    free(word);
    free(wordList);
    fprintf(stderr, "Memory Allocation Failure");
    exit(1);
    }
  */
  

  //check is fstat_obj is a regular file.
  //if it is, make a buffer that is the size of the input file.

  if(S_ISREG(fstat_obj.st_mode)){
    bigWordList = (char*)malloc(fstat_obj.st_size*sizeof(char));
    ssize_t fstate = read(0, bigWordList, fstat_obj.st_size);
    int wordLength = 1;
    if(fstate > 0){
      for(size_t i = 0 ; i < fstat_obj.st_size; i+=wordLength){
	wordLength = 1;
	if(i == fstat_obj.st_size - 1){
	  
	   bigWordList[i+1] = ' ';
	   }
	 if(bigWordList[i] == ' '){
	    for(size_t j = i; bigWordList[j]!= ' '; j++){
	      wordLength++;
	    }
	    wordCount++;
	 }
	 //wordCount++;
      }
      //now make every pointer in wordList point to the start
      //of the all the words in bigWordList.
      wordList = (char**)malloc(sizeof(char*)*(wordCount+1));
       if(wordList == NULL){
	  free(word);
	  free(wordList);
	  fprintf(stderr, "Memory Allocation Failure");
	  exit(1);
       }
      int wordCheck = 0;
      wordCount = 0;
      for(size_t j = 0; j < fstat_obj.st_size; j++){
	if(wordCheck == 0 && bigWordList[j] != ' '){
	  wordList[wordCount] = &bigWordList[j];
	  wordCount++;
	  wordCheck = 1;
	  continue;
	}if(wordCheck == 1 && bigWordList[j] == ' '){
	  wordCheck = 0;
	}
	/*	if(j == fstat_obj.st_size - 1){
	  wordList[wordCount] = &bigWordList[j];
	  wordCount++;
	  }*/
      }
      
    }
  }else{
    wordList = (char**)malloc(sizeof(char*)*WLFactor);
    if(wordList == NULL){
      free(word);
      free(wordList);
      fprintf(stderr, "Memory Allocation Failure");
      exit(1);
    }
    
  }
  char cur[1];
  ssize_t curState = 1;
  //have to figure out this size
  if(S_ISREG(fstat_obj.st_mode)){
    WLFactor = fstat_obj.st_size;
    curState =0;
  }else{
    WLFactor = 30;
  }
    
  
  while(curState > 0){
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
    curState = read(0, cur, 1);
    readError(curState);
    
    /*cur = getchar();
    if(ferror(stdin)!=0){
      fprintf(stderr, "error with I/O");
      }*/
  
    if(curState > 0){
      word[letterCount] = cur[0];
      letterCount++;
    }else{
      word[letterCount] = ' ';
      letterCount++;
      wordList[wordCount] = word;
      wordCount++;
    }
    
    if(cur[0] == ' '){

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
  
  
int wordSize = 0;
  
  for(int i = 0; i < wordCount; i++){
    wordSize = 0;
    for(int j = 0; wordList[i][j] != ' '; j++){
      wordSize++;
      
    }
    wordSize++;
    
    if(write(1, wordList[i], wordSize) == 0){
	fprintf(stderr, "write error");
	exit(1);
    }
  }/*
  printf("%d", wordCount);
  for (int i =0; i<wordCount; i++){
    printf("%s", wordList[i]);
    printf("%d", 0000);
    // free(wordList[i]);
    }*/
  free(wordList);
  exit(0);
}
