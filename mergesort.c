/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){
	//int size = rightend - leftstart + 1;
	int left = leftstart;
	int right = rightstart;
	int index = leftstart;
	while (left <= leftend && right <= rightend) {
		if (A[left] <= A[right]) {
			B[index] = A[left];
			left = left + 1;
		} else {
			B[index] = A[right];
			right = right + 1;
		}
		index = index + 1;
	}

	while (left <= leftend) {
		B[index] = A[left];
		index = index + 1;
		left = left + 1;
	};
    while (right <= rightend) {
    	B[index] = A[right];
    	index = index + 1;
    	right = right + 1;
    };

    // Copy result back
    int l;
    for (l = leftstart; l <= rightend; l++) {
        A[l] = B[l];
    }
}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right){
	//printf("Start my_mergesort left: %d right: %d \n", left, right);

	if (left >= right) {
		return;
	}

	int middle = (left + right) / 2;

	my_mergesort(left, middle);
	my_mergesort(middle + 1, right);
	merge(left, middle, middle + 1, right);
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){	
	struct argument *my_arg_pointer = (struct argument *)arg;
	
	int left = my_arg_pointer->left;
	int right = my_arg_pointer->right;
	int level = my_arg_pointer->level;
	int mid = (left + right) / 2;

	//printf("Start parallel_mergesort left: %d right: %d level: %d \n", left, right, level);
	if (level < cutoff) {
		pthread_t tid1, tid2;
		

        struct argument *args1 = buildArgs(left, mid, level + 1);
        struct argument *args2 = buildArgs(mid + 1, right, level + 1);	
        

        pthread_create(&tid1, NULL, parallel_mergesort, args1);
        pthread_create(&tid2, NULL, parallel_mergesort, args2);

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        //free(args1);
        //free(args2);
        merge(left, mid, mid + 1, right);
	} else {
		my_mergesort(my_arg_pointer->left, my_arg_pointer->right);	
	}

	
	//free(arg);
	//printf("Finished parallel_mergesort\n");
	return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
	//printf("Test build\n");
	struct argument *args = malloc(sizeof(struct argument));	
	args->left = left;
	args->right = right;
	args->level = level;
	//printf("Test build\n");
	return args;
}

