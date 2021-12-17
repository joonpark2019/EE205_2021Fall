#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "count.h"


#define UNIT_ARRAY_SIZE 1024
#define MAX_LINE 4096
#define MAX_WORD 1024

enum {HASH_MULTIPLIER = 65599};
enum {OUTSIDE, INSIDE};


//struct Word: data struct for storing the word string and the word's frequency
struct Word {
    char *word;                // actual word
    int freq;              // frequency found in stdin stream
  struct Word *nextWord; //pointer to next Node for Id hash table
};

/* struct WordTable contains both the hash table (lookupTable) for looking up 
new words as well as a Word array storeTable for storing distinct words
to be sorted by quicksort later */
struct WordTable {
    struct Word **storeTable;
    struct Word **lookupTable;
    int lookupSize;
    int storeSize; //size of storeTable
    int numWords; //number of distinct words encountered so far
};


//Hash function:
static int hash_function(const char *pcKey, int iBucketCount)
/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)(uiHash % (unsigned int)iBucketCount);
}

//returns -1 if successful
//return 0 if realloc failed or if some failure occurred
int lookupWord(WT t, const char *str){


    assert(t); assert(str);
    if(t==NULL || str == NULL)
      return 0;

    //first, search for the word in the hash table
    struct Word *w;
    int h = hash_function(str, t->lookupSize);
    //at the hash index h, traverse through linked list and see if a match is found
    for (w = t->lookupTable[h]; w!= NULL; w = w->nextWord){
        if(strcmp(w->word, str) == 0){
            w->freq++; //increase frequency of previous word and return
            return -1;
        }
    }

    //resize the storeTable if it is too short:
    struct Word** newTable; assert(t->storeTable != NULL); assert(t->lookupTable != NULL);
    if(t->numWords == t->storeSize - 1){
        newTable = (struct Word**)realloc(t->storeTable, 2*(t->storeSize)*sizeof(struct Word*));
        if(newTable==NULL){
            fprintf(stderr, "Can't realloc memory for storeTable\n");
            //free(newTable);
            return 0; //failed to realloc: insufficient memory available
        }
        t->storeSize *= 2;
        t->storeTable = newTable;
    }

    //resize the hashtable if too short:
    if(t->numWords >= 0.7*(t->lookupSize)){
      newTable = (struct Word**)realloc(t->lookupTable, 2*(t->lookupSize)*sizeof(struct Word*));
      if(newTable==NULL){
            fprintf(stderr, "Can't realloc memory for hash table\n");
            //free(newTable);
            return 0; //failed to realloc: insufficient memory available
        }
      t->lookupSize *= 2;
	    t->lookupTable = newTable;
    }



    //if word does not exist in the hash table:
    //memory allocation for new word to be added:
    struct Word* new_w = (struct Word*)calloc(1, sizeof(struct Word));
    //initialize node data:
    new_w->freq = 1;
    new_w->word = strdup(str);

    //add new node to hash table(lookupTable):
    new_w->nextWord = t->lookupTable[h];
    t->lookupTable[h] = new_w;
    //and add new node to storeTable
    t->numWords++;
    t->storeTable[t->numWords - 1] = new_w;

    return -1;
}

/* CreateWT function creates a struct WordTable*/
WT CreateWT(void)
{
  /*Declare WT and assign memory 
  (initialize all entries of hash table to NULL) using calloc*/
  WT t;
  
  t = (WT) calloc(1, sizeof(struct WordTable));
  if (t == NULL) {
    free(t);
    fprintf(stderr, "Can't allocate a memory for WordTable\n");
    return NULL;
  }
  //t->lookupSize = 100*UNIT_ARRAY_SIZE; // start with 100*1024 buckets
  t->numWords = 0;

  return t;
}

//InitializeStore(WT t) allocates memory for the storeTable and the lookupTable
//return 0 if memory allocation fails
//return -1 if successfully creates the storeTable
void InitializeStore(WT t){
    struct Word** WordArray = (struct Word **)calloc(100*UNIT_ARRAY_SIZE, sizeof(struct Word*));
    
    if(WordArray == NULL){ //calloc fails:
        free(WordArray);
        fprintf(stderr, "Can't allocate a memory for storeTable\n");
        return ;
    }
    t->storeTable = WordArray;
    t->storeSize = 100*UNIT_ARRAY_SIZE;

    struct Word** LookupArray = (struct Word **)calloc(100*UNIT_ARRAY_SIZE, sizeof(struct Word*));
    if(LookupArray == NULL){ //calloc fails:
        free(LookupArray);
        fprintf(stderr, "Can't allocate a memory for hash table\n");
        return ;
    }
    t->lookupTable = LookupArray;
    t->lookupSize = 100*UNIT_ARRAY_SIZE;

    return ;
}

/* function frees all data allocated for the struct WordTable.
It frees all struct Word items in the lookupTable and the storeTable */
void DestroyWT(WT t){

    struct Word *tWord;
    struct Word *nextTWord;

    for(int i=0; i < t->lookupSize; i++){
        for(tWord=t->lookupTable[i]; 
            tWord != NULL; tWord = nextTWord){
                nextTWord = tWord->nextWord;
                free(tWord->word);
                free(tWord);
        }
    }
    free(t->lookupTable);
    free(t->storeTable);
    free(t);

    return;
}


//comparator function for qsort:
int comparator(const void *p, const void *q)
{
    struct Word *data1 = *(struct Word **)p;
    struct Word *data2 = *(struct Word **)q;

    return data2->freq - data1->freq; //for descending order
}


/******************************* MAIN FUNCTION ***************************************/


int main(int argc, char* argv[])
{
	char buf[MAX_LINE], *temp;
	int  i, in_word = OUTSIDE;
	char str[MAX_WORD];
  
  struct timespec start_time, end_time;
  
 
  int fcheck;//integer receiving return value of lookupWord: checks if realloc fails
  int n; //n is the number of words to print out to stdout
  WT WordCounter = CreateWT();
  InitializeStore(WordCounter);

  //initialize clock:
  clock_gettime(CLOCK_MONOTONIC, &start_time);

	/* read a line from stdin, break it into words,
	   and print out each word at one line */
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		for (temp = buf; *temp; temp++) {
			if (isalpha(*temp)) {
				if (in_word == OUTSIDE) {
					in_word = INSIDE;
					i = 0;
				}
				str[i++] = *temp;
			} else {
				if (in_word == INSIDE) {
					str[i] = 0;
          fcheck = lookupWord(WordCounter, str);
					assert(fcheck != 0);
				}
				in_word = OUTSIDE;
			}
		}
	}
	if (in_word == INSIDE) {
		str[i] = 0;
		fcheck = lookupWord(WordCounter, str);
		assert(fcheck != 0);
	}

  /*check if command-line argument is provided and 
  assign the command-line argument to n if it is present */
  if(argc > 1){
    n = atoi(argv[1]);
  }else{
    n = WordCounter->numWords;
  }

  qsort(WordCounter->storeTable, WordCounter->numWords, sizeof(struct Word*), comparator);
  struct Word* w_temp;
  for(int i = 0; i < n; i++){
      w_temp = WordCounter->storeTable[i];
      fprintf(stdout, "%7d %s\n", w_temp->freq, w_temp->word);
  }
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  //printf("numWords: %d\n", WordCounter->numWords);
  //printf("storeSize: %d\n", WordCounter->storeSize);

  double time = (end_time.tv_sec - start_time.tv_sec)*1000000000 + 
                                (end_time.tv_nsec - start_time.tv_nsec);
  fprintf(stderr, "It took %.6lf seconds\n", time/1000000000);

  DestroyWT(WordCounter);

  return 0;
}
