#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>



void openstuff(DIR *ptr,const char *path){
	DIR *dirp;
	struct dirent *temp;
	char *pathname;
	pathname = malloc(sizeof(char)* 256);


	while((temp = readdir(ptr)) != NULL){
	//everytime run the loop, rewrite pathname
	//just in case it is overuse by last directory 
	strcpy(pathname,path);

	if((strcmp(temp->d_name, ".")) == 0){
	}else if((strcmp(temp->d_name, "..")) == 0){
	}else{

		if(temp->d_type == DT_DIR){
			//put / and the directoy name after path
			//use it to recursive open directoy 
			strcat(pathname,"/");
			strcat(pathname,temp->d_name);

			//if can open directory call openstuff it self
			if(dirp = opendir(pathname)){
			openstuff(dirp,pathname);
			}else{
			printf("can't open directory: %s",temp->d_name);
			}
		}else if(temp->d_type == DT_REG){
		//if the file is text, print file name and path
		printf("%s/%s\n",path,temp->d_name);
		}
	}
	}
}
int main(){
DIR *dirp;
size_t lineSize = 256;
char *keyword;
char *inputstring;
char *searchterm;
char *dir;
char ch = 0;
int stringindex = 0;
int index = 0;
int argc = 0;
int returnval;
int check = 0;
inputstring = malloc(lineSize * sizeof(char));
keyword = malloc(lineSize * sizeof(char));
searchterm = malloc(lineSize * sizeof(char));
dir = malloc(lineSize * sizeof(char));

getline(&inputstring,&lineSize,stdin);
while(ch != '\n'){
ch = inputstring[stringindex];
	if(argc == 0){
		//first part of the arguement
		//which is file_search
		//just in case user input something else
		//read and check the keyword
		if(isalpha(ch) || ch == '_'){
		keyword[index] = ch;
		index++;
		}else{
			if(index == 0){
			printf("invalid input");
			}else{
			index = 0;
			argc++;
			}
		}

	}else if (argc == 1){
		//second argument
		//which will be the word to search
		//allow user to enter 1 word
		//or use quotation mark to enter more than 1 word

		if(check == 1){
			if(ch == '"'){
			check++;
			}else{
			searchterm[index] = ch;
			index++;
			}

		}else{
			if(ch == '"'){
			check++;
			}else if(ch == ' '){
			check = 2;
			}else{
			searchterm[index] = ch;
			index++;
			}
		}

		if(check == 2){
			index = 0;
			argc++;
		}

	}else if (argc == 2){
		//read in everything,
		//only check is the input char is \n
		//read 
		if(ch == ' '){
		argc++;
		}else if (ch == '\n'){
		}else{
		dir[index] = ch;
		index++;
		}


	}else{
	printf("Invalid input: Too many argument\n");
	return 0;
	}
stringindex++;
}
//check is the first char of path is /
//if not, print invalid input
if(dir[0] != '/'){
printf("Invalid input: Invalid direction\n");
return 0;
}

//check is the last char of path is /
//if it is / , print invalid input
if(dir[index] == '/'){
printf("Invalude input: end of direction can't be /\n");
return 0;
}

//start loop

if(dirp = opendir(dir)){
openstuff(dirp,dir);
}else{
printf("can't open directory\n");
}

//============
printf("part 1:%s\n",keyword);
printf("part 2:%s\n",searchterm);
printf("part 3:%s\n",dir);
//=================
free(inputstring);
free(keyword);
free(searchterm);
free(dir);

return 0;
}
