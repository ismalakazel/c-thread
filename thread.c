#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*
 * To compile this file you must pass the argument `-pthread` to gcc. eg:
 * 
 * $ gcc -pthread thread.c
 *
 * To compile and run the program use the following:
 *
 * $ gcc -pthread thread.c && ./a.out yourstringhere
 */

/// Convenient way to handle error codes.
#define handle_error(message) \
		do { perror(message); exit(EXIT_FAILURE); } while (0) 

/// A struct to be given to pthread_create. 
struct thread_info {
	pthread_t thread_id; 
	int 	  thread_number;
	char 	  *name;
};

/// A function to be given to pthread_create.
static void *routine(void *arguments) {	
	struct thread_info *thread_information = arguments;
	printf("thread id = %ld \n", thread_information->thread_id);
	printf("thread number = %d \n", thread_information->thread_number);
	printf("thread name = %s \n", thread_information->name);
};

int main(int argc, char *argv[]) {

	if (argv[1] == NULL) {
		printf("No arguments given to *argv[].\n");
		exit(EXIT_FAILURE);	
	};

	/// Create and allocate a thread_info struct. Remember, it needs to be freed.
	struct thread_info *thread_information = malloc(sizeof(struct thread_info));
	char name[strlen(argv[1])];
	strcpy(name, argv[1]);
	thread_information->name = name;
	thread_information->thread_id = 0;
	thread_information->thread_number = 0;

	/// Thread attributes to be given to pthread_create. Remember, it should be destroyed after pthread_create.
	pthread_attr_t thread_attributes;
	int thread_attributes_result = pthread_attr_init(&thread_attributes);
	if (thread_attributes_result != 0) {
		handle_error("create attributes");	
	};

	/// Create new thread.
	int thread = pthread_create(&thread_information->thread_id, &thread_attributes, &routine, &thread_information);
	if (thread != 0) {
		handle_error("thread join");
	};
	
	/// Destroy attributes.
	pthread_attr_destroy(&thread_attributes);

	/// Wait for the thread to terminate.
	int thread_join = pthread_join(thread_information->thread_id, NULL);
	if (thread_join != 0) {
		handle_error("thread join");
	};

	/// Free allocated memory for thread_information.
	free(thread_information);

	/// Tchau.
	exit(EXIT_SUCCESS);
	
	return 0;
}
