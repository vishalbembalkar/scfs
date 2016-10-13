#include<stdio.h> 
#include<string.h>
#include<unistd.h>
int main(){


char buffer[255]="pwd";
FILE *ls=popen(buffer,"r");
	do{
	if(feof(ls))
		{
		printf("\nEOF hello!!");
		break;
		}
	while(fgets(buffer,sizeof(buffer),ls)!=0){
	printf("\nPYTHON_ %s",buffer);
		
	}
	}while(1)	;
	pclose(ls);
	return 0;
	}
