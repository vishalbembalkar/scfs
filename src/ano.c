#include<stdio.h>
#include<string.h>

int main(){
char anobuffer[4096]="/home/poizen/scfs_portable_module/src/b/01 - Bloodletting.mp3";
int i=0,len=sizeof(anobuffer);
for(i=0;i<=len;i++){
if(anobuffer[i]==' '){

anobuffer[i]='#';

}


}

printf("\n%s", anobuffer);

return 0;
}
