#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#include <sys/type.h>
//#include <dirent.h>
int main(){
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
		keyword = realloc(str,);
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

		//variable check use fo check did user input quotation mark
		//if user didn't input quotation mark check will be 0
		//check = 1 mean user already input quotation mark
		//when system read aonother quotation mark check will be 2
		//if used only enter 1 word,
		//check = 2 when read another space

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
//			printf("readin:%c\n",ch);
			}
		}

		if(check == 2){
			index = 0;
			argc++;
		}

	}else if (argc == 2){
		//read in everything,
		//only check is the input char is \n
		//read everything until  \n
		if(ch == '\n'){

		argc++;
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
if(dir[0] != '/'){
printf("Invalid input: Invalid direction\n");
return 0;
}
if(dir[index] == '/'){
printf("Invalude input: end of direction can't be /\n");
return 0;
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
