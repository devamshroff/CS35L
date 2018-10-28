#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int helperSearch(char ch, char* from);

int main(int argc, char* argv[])
{
  //argument length
  if(argc != 3){
    fprintf(stderr, "argument length has to be 2");
    exit(1);
  }
  
  char* from = argv[1];
  char* to = argv[2];

  //from length and to length
  if(strlen(from)!=strlen(to)){
    fprintf(stderr, "map to should have the same size as map from");
    exit(1);
  }

  //from duplicates
  for(int i = 0; i < strlen(from); i++){
    for(int j = i+1; j < strlen(from); j++){
      if(from[i] == from[j]){
	fprintf(stderr, "from has duplicates");
	exit(1);
      }
    }

  }
  char ch = getchar();
  int temp = -1;
  
  while(ch!=EOF){
    temp = helperSearch(ch, from);
    if(temp == -1){
      putchar(ch);
    }else{
      putchar(to[temp]);
    }
    ch = getchar();
    if(ferror(stdin)!=0){
      fprintf(stderr, "error with I/O");
    }
  }
}

int helperSearch(char ch, char* from){
  int len = strlen(from);

  for(int i = 0; i < len; i++){
    if(from[i] == ch){
      return i;
    }
  }
  return -1;
}




  
  
