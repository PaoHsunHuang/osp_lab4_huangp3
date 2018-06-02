#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <dirent.h> 
#include <string.h> 
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>
#define _GNU_SOURCE
#define threadNum 4
#define maxline 256
void *recur_file_search(void *file);
void recur_file_search2(char *file2);

struct arginfo{
	char *intstr;
	char *outstr;
};

char *name[threadNum];

int threadIndex = 0;
const char *search_term;
pthread_t *threads;
//int avaliable[threadNum];
void *returnVal[threadNum];
struct arginfo *cp[threadNum];
struct arginfo *tempcp;

char *next_file_str;

//takes a file/dir as argument, recurses,
// prints name if empty dir or not a dir (leaves)
//share search term globally (rather than passing recursively)

int main(int argc, char **argv) {

	struct arginfo *startfile = malloc(sizeof(struct arginfo));

	//create two thread
	threads =(pthread_t*)malloc(sizeof(pthread_t)*threadNum);
	for(int i = 0;i < threadNum;i++){
	cp[i] = malloc(sizeof(struct arginfo));
	cp[i]->outstr = malloc(sizeof(char) * maxline);
	cp[i]->intstr = malloc(sizeof(char) * maxline);
	}
	tempcp = malloc(sizeof(struct arginfo));
	tempcp->outstr = malloc(sizeof(char) * maxline);
	tempcp->intstr = malloc(sizeof(char) * maxline);


	if(argc != 3)
	{
		printf("Usage: my_file_search <search_term> <dir>\n");
		printf("Performs recursive file search for files/dirs matching\
				<search_term> starting at <dir>\n");
		exit(1);
	}

	//don't need to bother checking if term/directory are swapped, since we can't
	// know for sure which is which anyway
	search_term = argv[1];

	//open the top-level directory
	DIR *dir = opendir(argv[2]);

	//make sure top-level dir is openable (i.e., exists and is a directory)
	if(dir == NULL)
	{
		perror("opendir failed");
		exit(1);
	}

	//start timer for recursive search
	struct timeval start, end;
	gettimeofday(&start, NULL);

	startfile->intstr = argv[2];

	recur_file_search((void*)startfile);



//	printf("after file\n");


	gettimeofday(&end, NULL);
	printf("Time: %ld\n", (end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec));
	return 0;
}

//This function takes a path to recurse on, searching for mathes to the
// (global) search_term.  The base case for recursion is when *file is 
// not a directory. 
//Parameters: the starting path for recursion (char *), which could be a 
// directory or a regular file (or something else, but we don't need to 
// worry about anything else for this assignment).
//Returns: nothing 
//Effects: prints the filename if the base case is reached *and* search_term 
// is found in the filename; otherwise, prints the directory name if the directory 
// matches search_term.
void *recur_file_search(void *file)
{
	if(threadIndex == threadNum){
	threadIndex = 0;
	}
	printf("thread(%d)\n",threadIndex);
	tempcp = file;
//	printf("initial:%s\n",cp2[threadIndex]->intstr);

	name[threadIndex] = tempcp->intstr;
//	printf("find address %s\n",name[threadIndex]);





	//check if directory is actually a file
	DIR *d = opendir(name[threadIndex]);
	//NULL means not a directory (or another, unlikely error)
	if(d == NULL)
	{
		//opendir SHOULD error with ENOTDIR, but if it did something else,
		// we have a problem (e.g., forgot to close open files, got
		// EMFILE or ENFILE)
		if(errno != ENOTDIR)
		{
			perror("Something weird happened!");
			fprintf(stderr, "While looking at: %s\n", name[threadIndex]);
			exit(1);
		}

		//nothing weird happened, check if the file contains the search term
		// and if so print the file to the screen (with full path)
		if(strstr(name[threadIndex], search_term) != NULL)
			printf("%s\n", name[threadIndex]);
		//no need to close d (we can't, it is NULL!)

	}

	//we have a directory, not a file, so check if its name
	// matches the search term
	if(strstr(file, search_term) != NULL)
		printf("%s/\n", name[threadIndex]);



	//call recur_file_search for each file in d
	//readdir "discovers" all the files in d, one by one and we
	// recurse on those until we run out (readdir will return NULL)
	struct dirent *cur_file;
	while((cur_file = readdir(d)) != NULL)
	{
		//make sure we don't recurse on . or ..
		if(strcmp(cur_file->d_name, "..") != 0 &&
				strcmp(cur_file->d_name, ".") != 0 &&
				cur_file->d_type == DT_DIR)
		{
			//we need to pass a full path to the recursive function,
			// so here we append the discovered filename (cur_file->d_name)
			// to the current path (file -- we know file is a directory at
			// this point)
			
			char *next_file_str = malloc(sizeof(char) * 
					strlen(cur_file->d_name) + 
					strlen(name[threadIndex]) + 2);

			strncpy(next_file_str, name[threadIndex], strlen(name[threadIndex]));
			strncpy(next_file_str + strlen(name[threadIndex]), 
					"/", 1);
			strncpy(next_file_str + strlen(name[threadIndex]) + 1, 
					cur_file->d_name, 
					strlen(cur_file->d_name) + 1);
			//recurse on the file


//			printf("new path:%s\n",next_file_str);

			cp[threadIndex]->intstr = next_file_str;
			//strcpy(cp[threadIndex]->intstr,next_file_str);

//			printf("thread(%d) path: %s\n",threadIndex,cp[threadIndex]->intstr);


			//check is there are any avalibe thread

			for(int k = 0;k < threadNum;k++){

			if(pthread_join(threads[k],returnVal[k]) == 0){
			printf("use thread(%d)\n",k);
//			printf("thread(%d):%d\n",k,returnVal);
			pthread_create(&threads[threadIndex], NULL, recur_file_search, (void*)cp[threadIndex++]);

			}else{
//			printf("normal path\n",k);
			printf("pass to normal:%s\n",cp[threadIndex]->intstr);
//			recur_file_search2(cp[threadIndex]->intstr);

			}

			}





//			recur_file_search((void*)&next_file_str);
			//free the dynamically-allocated string
			free(next_file_str);

		}

		if(){


		}


		


	}

	//close the directory, or we will have too many files opened (bad times)
	closedir(d);
}

//This function takes a path to recurse on, searching for mathes to the
// (global) search_term.  The base case for recursion is when *file is 
// not a directory.
//Parameters: the starting path for recursion (char *), which could be a
// directory or a regular file (or something else, but we don't need to 
// worry about anything else for this assignment).
//Returns: nothing
//Effects: prints the filename if the base case is reached *and* search_term
// is found in the filename; otherwise, prints the directory name if the directory 
// matches search_term.

 void recur_file_search2(char *file2) {
//check if directory is actually a file
	printf("normal search\n");
	DIR *dir = opendir(file2);
	//NULL means not a directory (or another, unlikely error)
	if(dir == NULL)
	{
		//opendir SHOULD error with ENOTDIR, but if it did something else,
		// we have a problem (e.g., forgot to close open files, got
		// EMFILE or ENFILE)
		if(errno != ENOTDIR)
		{
			perror("Something weird happened!");
			fprintf(stderr, "While looking at: %s\n", file2);
			exit(1);
		}
		//nothing weird happened, check if the file contains the search term
		// and if so print the file to the screen (with full path)
		if(strstr(file2, search_term) != NULL)
			printf("%s\n", file2);
		//no need to close d (we can't, it is NULL!)
		return;
	}
	//we have a directory, not a file, so check if its name
	// matches the search term
	if(strstr(file2, search_term) != NULL)
		printf("%s/\n", file2);
	//call recur_file_search for each file in d
	//readdir "discovers" all the files in d, one by one and we
	// recurse on those until we run out (readdir will return NULL)
	struct dirent *cur_file;
	while((cur_file = readdir(dir)) != NULL)
	{
		//make sure we don't recurse on . or ..
		if(strcmp(cur_file->d_name, "..") != 0 &&\
				strcmp(cur_file->d_name, ".") != 0)
		{
			//we need to pass a full path to the recursive function,
			// so here we append the discovered filename (cur_file->d_name)
			// to the current path (file -- we know file is a directory at
			// this point)
			char *next_file_str = malloc(sizeof(char) * \
					strlen(cur_file->d_name) + \
					strlen(file2) + 2);
			strncpy(next_file_str, file2, strlen(file2));
			strncpy(next_file_str + strlen(file2), \
					"/", 1);
			strncpy(next_file_str + strlen(file2) + 1, \
					cur_file->d_name, \
					strlen(cur_file->d_name) + 1);
			//recurse on the file
			recur_file_search(next_file_str);
			//free the dynamically-allocated string
			free(next_file_str);
		}
	}
	//close the directory, or we will have too many files opened (bad times)
	closedir(dir);
}
