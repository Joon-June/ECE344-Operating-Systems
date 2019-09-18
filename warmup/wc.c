#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "wc.h"

struct pair{
	int keySize;
	int value; // count in this case
	struct pair* next;
	char* key;
};

struct wc {
	/* you can define this struct to have whatever fields you want. */
	int tableSize;
	struct pair** hashTable;
	
};
/* Maybe this is a good hash function*/
unsigned long generateHashCode(char *str);
int generateIdx(unsigned long hashCode, int tableSize);

/* Forward Declarations */

// void lowerString(char* argStr){

// 	while( *argStr != '\0'){
// 		*argStr = tolower(*argStr);
// 		argStr++;
// 	}

// }

// bool isLetter(char argChar){

// 	if(isspace(argChar)){
// 		return false;
// 	}

// 	if( (argChar > 96 && argChar < 123) || 
// 		(argChar > 64 && argChar < 91) ){
// 		return true;
// 	}

// 	return false;

// }


struct wc *
wc_init(char *word_array, long size)
{
	struct wc *wc;

	wc = (struct wc *)malloc(sizeof(struct wc));
	assert(wc);

	wc->tableSize = size/4; // initialize hash table size with 2*Number of total words
	wc->hashTable = (struct pair**)malloc(sizeof(struct pair*) * wc->tableSize);
	memset(wc->hashTable, '\0', wc->tableSize*sizeof(struct pair*));


	// Get the word size
	int startingIdx;
	for(int endIdx = 0; endIdx < size; endIdx++){

		// Find the next non-space
		while(isspace(word_array[endIdx])){
			endIdx++;
		}
		
		startingIdx = endIdx; // change the startingIdx for the next word

		// Find the next space
		while(!isspace(word_array[endIdx])){
			endIdx++;
		}

		// 1. Parse a word from the given array
		int strLen = endIdx-startingIdx;
		char* currentWord = (char*)malloc(sizeof(char)*(strLen+1)); // Allocate memory for current word
		memcpy(currentWord, word_array + startingIdx, sizeof(char)*strLen); 	// Parse the current word
		currentWord[strLen] = '\0'; // end of string

		// 2. Generate a hash code and index
		unsigned long hash = generateHashCode(currentWord);
		int idx = generateIdx(hash, wc->tableSize);

		// 3. Check if the current word already exists
		if(wc->hashTable[idx] == NULL){

			// 3-1. The word does not exist. Insert into the table.
			struct pair* wordObj = (struct pair*)malloc(sizeof(struct pair));
			wordObj->key = (char*)malloc(sizeof(char)*(strLen+1));
			strcpy(wordObj->key, currentWord);
			wordObj->keySize = strLen;
			wordObj->value = 1;
			wordObj->next = NULL;

			wc->hashTable[idx] = wordObj;

		}
		else{
			// 4. Check chains
			struct pair* curr = wc->hashTable[idx];
			struct pair* prevPair;

			while(1){
				// If the word already exists, increase the count
				if(strcmp(curr->key, currentWord) == 0){
					curr->value++;
					break;
				}
				else{
					prevPair = curr;
					curr = curr->next;

					if(curr == NULL){
						struct pair* wordObj = (struct pair*)malloc(sizeof(struct pair));
						wordObj->key = (char*)malloc(sizeof(char)*(strLen+1));
						strcpy(wordObj->key, currentWord);
						wordObj->keySize = strLen;
						wordObj->value = 1;
						wordObj->next = NULL;

						prevPair->next = wordObj; // link to the previous node

						break;
					}
				}
			}

		}

		free(currentWord);
	}
	// 2. Loop through the array and increase the count in hashTable

	return wc;
}

void
wc_output(struct wc *wc)
{	
	for(int i = 0; i < wc->tableSize; i++){
		if(wc->hashTable[i] != NULL){
			struct pair* curr = wc->hashTable[i];

			while(curr != NULL){
				printf("%s:%d\n", curr->key, curr->value);
				curr = curr->next;
			}
		}
	}
}

void
wc_destroy(struct wc *wc)
{
	for(int i = 0; i < wc->tableSize; i++){
		if(wc->hashTable[i] != NULL){
			struct pair* curr = wc->hashTable[i];
			struct pair* prev;

			while(curr != NULL){
				prev = curr;
				curr = curr->next;

				free(prev);
			}
		}
	}
	free(wc->hashTable);
	free(wc); // not sure if it also frees dynamically allocated elements
}

/* This hash funciton is not my work*/
/* URL: https://stackoverflow.com/questions/7666509/hash-function-for-string */
unsigned long
generateHashCode(char *str)
{
    unsigned long hash = 5381;
    int c;

    while( (c = *str++) ){
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

    return hash;
}

int generateIdx(unsigned long hashCode, int tableSize){
	return hashCode % tableSize;
}
