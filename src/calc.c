#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>

int main(){
FILE *try;
char buffer[1024]="python /home/poizen/scripting/testpath.py '/home/poizen/scfs_portable_module/src/z/music/supercell/Guilty Crown ED2 & Black Rock/Bokura no Ashiato.mp3'";
char c[10]="101";
try=popen(buffer,"r");
if(strcmp(buffer,c)==0){
printf ("%s",c);
}

return 0;
}
