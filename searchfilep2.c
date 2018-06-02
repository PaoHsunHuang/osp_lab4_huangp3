#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <dirent.h> 
#include <string.h> 
#include <sys/time.h> 
#include <pthread.h>
#include <errno.h>
#define threadNum 4
#define max 80
//takes a file/dir as argument, recurses, // prints name if empty dir or not a dir (leaves)

void *recur_file_search(void *file); 

struct pathinfo{
	char *path;
	int threadid;
};
struct pathinfo *threadarg[threadNum];
struct pathinfo *output;
struct pathinfo *cp;


pthread_t threads[threadNum];

//share search term globally (rather than passing recursively) 
const char *search_term; 
int main(int argc, char **argv) {


	for(int i = 0; i < threadNum; i++){
	threadarg[i] = malloc(sizeof(struct pathinfo));
	threadarg[i]->path = malloc(sizeof(char) * max);
	}
	output = malloc(sizeof(struct pathinfo));
	output->path = malloc(sizeof(char) * max);
	cp = malloc(sizeof(struct pathinfo));
	cp->path = malloc(sizeof(char) * max);
	



//	cp->path = malloc(sizeof(char) * max);
//	output = malloc(sizeof(struct pathinfo));
//	output->path = malloc(sizeof(char) * max);

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
	threadarg[0]->path = argv[2];
	threadarg[0]->threadid = 0;
//	printf("path %s\n",threadarg[0]->path);
//	printf("threadid %d\n",threadarg[0]->threadid);
//	printf("call function\n");


	pthread_create(&threads[0],NULL, recur_file_search,(void*)threadarg[0]);
//	recur_file_search((void*) &threadarg[0]);

	pthread_join(threads[0],NULL);
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
void *recur_file_search(void *file) {
//check if directory is actually a file
//	free(pathcp);
	char *pathcp;
//	pathcp = malloc(sizeof(char) * max);

	cp = file;
	pathcp = malloc(sizeof(char) * max);
//	printf("pathcp:%s\n", pathcp);
	pathcp = cp->path;
//	printf("open:%s\n", pathcp);


	DIR *d = opendir(pathcp);
	//NULL means not a directory (or another, unlikely error)
	if(d == NULL)
	{
		//opendir SHOULD error with ENOTDIR, but if it did something else,
		// we have a problem (e.g., forgot to close open files, got
		// EMFILE or ENFILE)
		if(errno != ENOTDIR)
		{
			perror("Something weird happened!");
			fprintf(stderr, "While looking at: %s\n", pathcp);
			exit(1);
		}
		//nothing weird happened, check if the file contains the search term
		// and if so print the file to the screen (with full path)
		if(strstr(pathcp, search_term) != NULL)
			printf("%s\n", pathcp);
		//no need to close d (we can't, it is NULL!)

		return NULL;
	}
	//we have a directory, not a file, so check if its name
	// matches the search term
	if(strstr(pathcp, search_term) != NULL)
		printf("%s/\n", pathcp);
	//call recur_file_search for each file in d
	//readdir "discovers" all the files in d, one by one and we
	// recurse on those until we run out (readdir will return NULL)
	struct dirent *cur_file;
	while((cur_file = readdir(d)) != NULL)
	{
		//make sure we don't recurse on . or ..
		if(strcmp(cur_file->d_name, "..") != 0 &&\
				strcmp(cur_file->d_name, ".") != 0)
		{
			//we need to pass a full path to the recursive function,
			// so here we append the discovered filename (cur_file->d_name)
			// to the current path (file -- we know file is a directory at
			// this point)
//			printf("alloc memory\n");
			char *next_file_str = malloc(sizeof(char) * 
					strlen(cur_file->d_name) + 
					strlen(pathcp) + 2);
//			printf("copy old string: %s\n", pathcp);

			strncpy(next_file_str,pathcp, strlen(pathcp));

			strncpy(next_file_str + strlen(pathcp), 
					"/", 1);
			strncpy(next_file_str + strlen(pathcp) + 1, 
					cur_file->d_name, 
					strlen(cur_file->d_name) + 1);

			//recurse on the file
//			printf("next file name: %s\n", next_file_str);
//			printf("==============================================\n");
			output->path = next_file_str;
//			printf("output path: %s\n", output->path);
			recur_file_search((void*)output);
			//free the dynamically-allocated string
			free(next_file_str);
		}
	}
	//close the directory, or we will have too many files opened (bad times)
//	free(pathcp);
	closedir(d);
}
