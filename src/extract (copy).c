#include<stdio.h>
#include<sys/stat.h> 
#include<string.h>

#include<stdlib.h>
int main()
{
int i=0;
char *s1= "extract /home/poizen/Pictures/x.jpg | grep mime";
//system(s1);
char buf[1000],*arr[1000];
FILE *ls = popen(s1, "r");
printf("\n AFTER GETTING IT IN\n");
while (fgets(buf, sizeof(buf), ls) != 0) 
	{
 	printf("%d ->%s",i, buf);
	if(*arr="mimetype - image/jpeg")
		printf("file is an image" );
	
	
}


pclose(ls);
return 0;

}
