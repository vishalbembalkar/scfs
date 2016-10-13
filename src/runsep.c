#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
int main(){


FILE *c;
char anobuffer[2000],fpath[2000]="LOLWAZ";

strcpy(anobuffer,fpath);
printf("\n another_buffer: %s",anobuffer);
//replace space in fpath with '\ '
c=fopen("/home/poizen/readthis.txt","w");
if(c==NULL){
printf("\n To err is human");
error(1);
}
fprintf(c,"%s",anobuffer);
pclose(c);
return 0;
}

