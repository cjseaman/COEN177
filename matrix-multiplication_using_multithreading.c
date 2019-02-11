/*
 * Collin Seaman
 * COEN 177 LAB 4
 * 1/30/2018
 * Matrix multiplication using multithreading
 * This program multiplies two square matrices of size SIZE, initialized with random values, using multithreading
 * and compares the result to a single threaded implementataion of matrix multiplication
 * prints TRUE if results match, otherwise prints FALSE
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 1024

pthread_t threads[SIZE];

float matrix1[SIZE][SIZE];
float matrix2[SIZE][SIZE];
float result[SIZE][SIZE] = {0}; //multithreaded result
float check[SIZE][SIZE] = {0}; //single threaded result

//Single threaded implementation of matrix multiplication for error checking
void singlet(void) {
	unsigned i, j, k;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			for (k = 0; k < SIZE; k++)
				check[i][j] += matrix1[i][k] * matrix2[k][j];
}

//multithreaded function for matrix multiplication
void *mmultiply(void *args);

//checks that the result of our multithreaded and single threaded matrix multiplication is the same
int is_result_correct(void) {
	int i,j;
	for(i=0;i<SIZE;i++) {
		for(j=0;j<SIZE;j++) {
			if(result[i][j] != check[i][j]) {
				return 0;
			}
		}
	}
	return 1;
}

//generates a random value
int irand(int start, int end) {
	int range = end - start + 1;
	return rand() % range + start;
}

int main() {
	
    int i, j;
    int issame = 1;

//fill matrix1
    for (i = 0; i < SIZE; i++) {
	for(j=0; j < SIZE; j++) { 
	    matrix1[i][j] = (float) irand(1,5);
	}
    }  
//fill matrix2 
    for (i = 0; i < SIZE; i++) {
	for(j=0; j < SIZE; j++) { 
	    matrix2[i][j] = (float) irand(1,5);
	}
    } 

    printf("\nMatrix 1:");
//print matrix1
    for (i = 0; i < SIZE; i++) {
	printf("\n");
	for(j = 0; j < SIZE; j++) { 
	    printf("%f ", matrix1[i][j]);
	}
    }  
    
    printf("\nMatrix 2:");
//print matrix2
    for (i = 0; i < SIZE; i++) {
	printf("\n");
	for(j = 0; j < SIZE; j++) { 
	    printf("%f ", matrix2[i][j]);
	}
    }

    printf("\nCalculating...\n");
//calculate matrix multiplication of matrix1 and matrix2, fills check
    singlet();

//create threads
    for (i = 0; i < SIZE; i++){
        pthread_create(&threads[i], NULL, mmultiply, (void *)(size_t)i);
    }
//wait for threads to terminate
    for(i=0;i<SIZE;i++) {
	pthread_join(threads[i], NULL);
    }
    
    printf("\nMultithread Result:");
//prints multithreaded result    
    for (i = 0; i < SIZE; i++) {
	printf("\n");
	for(j = 0; j < SIZE; j++) { 
	    printf("%f ", result[i][j]);
	}
    }

    printf("\nSingle thread result:");
//prints single threaded result    
    for (i = 0; i < SIZE; i++) {
	printf("\n");
	for(j = 0; j < SIZE; j++) { 
	    printf("%f ", check[i][j]);
	}
    }
//issame = 1 if multi and single threaded results are the same    
    issame = is_result_correct();
    if(issame == 1) {
	printf("TRUE\n");
    }
    else printf("FALSE, issame is %d\n", issame);
}//end main

//calculates one row of result
void *mmultiply(void *args) {
    int i, j, k;
    int row = (int) args;
    for(i = 0; i < SIZE; i++) {
	for(j = 0; j < SIZE; j++) {
            result[row][i] += matrix1[row][j] * matrix2[j][i];	
	}
    }
    return (NULL);
}


