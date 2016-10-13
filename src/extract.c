#include<stdio.h>
#include<sys/stat.h> 
#include<string.h>

#include<stdlib.h>
int main()
{
int i=0;
char *temp=NULL,*str=NULL,*s1= "extract /home/poizen/Pictures/x.jpg "; //just pass some path where some file like .mp3 exists. 
unsigned int size=1;
unsigned int strlength;
//system(s1);
int c;
char buf[1000],*arr[1000];
FILE *ls = popen(s1, "r");

do{

c=fgetc(ls);
if(feof(ls))
{
break;
}
//printf("%c",c);
if(c=="m")
	

}while(1);

printf("\n after the loop  c is  : %c", c ) ;
//printf("\n AFTER GETTING IT IN\n");
/*while (fgets(buf,sizeof(buf),ls) != 0) 
	{
	strlength=strlen(buf);
	temp=realloc(str,size +strlength);
	if(temp==NULL){
	printf("allocation error\n") ;
	}
	else{
	str=temp;
	}
	strcpy(str+size-1,buf);
	size+=strlength;
	
	i++;
}

printf("\n%s " , str);*/

pclose(ls);
return 0;

}
