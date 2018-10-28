#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

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

  char ch[2];
  ssize_t readVal = read(0, ch, 1);
  while(readVal > 0){
    int temp = helperSearch(ch[0], from);
    if(temp == -1){
      write(1, ch, 1);
    }else{
      ch[0] = to[temp];
      write(1, ch, 1);  
    }
    readVal = read(0, ch, 1);
  }
    return 0;
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
