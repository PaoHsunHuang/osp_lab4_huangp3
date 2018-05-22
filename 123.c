#include <string.h>
#include <stdlib.h>
#include <unistd.h>  
#include <stdio.h>  
#include <sys/types.h>  
#include <dirent.h>  

void openstuff(DIR *ptr,const char *path){
	DIR *dirp;
	struct dirent *temp;
	char *pathname;

	pathname = malloc(sizeof(char)* 256);



	printf("from directory:%s\n",pathname);
	while((temp = readdir(ptr)) != NULL){
	strcpy(pathname,path);

	if((strcmp(temp->d_name, ".")) == 0){
	}else if((strcmp(temp->d_name, "..")) == 0){
	}else{

		if(temp->d_type == DT_DIR){
			strcat(pathname,"/");
			strcat(pathname,temp->d_name);
//			printf("dir:%s\n",dirname);
//			printf("path:%s\n",pathname);
//
			if(dirp = opendir(pathname)){
			openstuff(dirp, pathname);
			}else{
			printf("can't open directoy:%s\n",pathname);
			}
		}else if(temp->d_type == DT_REG){

		printf("%s/%s\n",path,temp->d_name);
		}else{
		printf("can't type: %s\n",temp->d_name);
		}

	}
	}

	free(pathname);
}

int main(){
char *file = "/home/huangp3/hw1";
DIR *dirp;

if(dirp = opendir(file)){
openstuff(dirp, file);
}else{
printf("can't open file\n");
}

return 0;
}
