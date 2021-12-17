#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "count.h"


#define UNIT_ARRAY_SIZE 1024
#define MAX_LINE 4096
#define MAX_WORD 1024

enum {HASH_MULTIPLIER = 65599};
enum {OUTSIDE, INSIDE};

struct Word {
  char *word;                // actual word
    int freq;              // frequency found in stdin stream
  struct Word *nextWord; //pointer to next Node for Id hash table
};

struct WordTable {
    struct Word **storeTable;
    struct Word *lookupTable[UNIT_ARRAY_SIZE];
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
    int h = hashfunction(str, t->lookupSize);
    for (w = t->lookupTable[h]; w!= NULL; w = w->nextWord){
        if(strcmp(w->word, str) == 0){
            w->freq++;
            return -1;
        }
    }

    //resize the storeTable if it is too short:
    struct Word** newTable; assert(t->storeTable != NULL);
    if(t->numWords == t->storeSize - 1){
        newTable = (struct Word**)realloc(t->storeTable, 2*(t->storeSize)*sizeof(struct Word*));
        if(newTable==NULL){
            fprintf(stderr, "Can't realloc memory for storeTable\n");
            return 0; //failed to realloc: insufficient memory available
        }
        t->storeSize *= 2;
        t->storeTable = newTable;
    }

    //if word does not exist in the hash table:
    //memory allocation for new word to be added:
    struct Word* new_w = (struct Word*)calloc(1, sizeof(struct Word));
    //initialize node data:
    new_w->freq = 0;
    new_w->word = strdup((char *)str);

    //add new node to hash table(lookupTable):
    new_w->nextWord = t->lookupTable[h];
    t->lookupTable[h] = new_w;
    //and add new node to storeTable
    t->storeTable[t->numWords] = new_w;
    t->numWords++;

    return -1;
}


WT CreateWT(void)
{
  /*Declare DB and assign memory 
  (initialize all entries of hash table to NULL) using calloc*/
  WT t;
  
  t = (WT) calloc(1, sizeof(struct WordTable));
  if (t == NULL) {
    free(t);
    fprintf(stderr, "Can't allocate a memory for WordTable\n");
    return NULL;
  }
  t->lookupSize = UNIT_ARRAY_SIZE; // start with 1024 buckets
  t->numWords = 0;

  return t;
}

//return 0 if memory allocation fails
//return -1 if successfully creates the storeTable
void InitializeStore(WT t){
    assert(t->storeTable != NULL);
    struct Word** WordArray = (struct Word **)calloc(UNIT_ARRAY_SIZE, sizeof(struct Word*));
    
    if(WordArray == NULL){ //calloc fails:
        free(WordArray);
        fprintf(stderr, "Can't allocate a memory for storeTable\n");
        return NULL;
    }
    t->storeTable = WordArray;
    t->storeSize = UNIT_ARRAY_SIZE;

    return ;
};


void DestroyWT(WT t){

    struct Word *tWord;
    struct Word *nextTWord;

    for(int i=0; i < t->lookupSize; i++){
        for(tWord=t->lookupTable[i]; 
            tWord != NULL; tWord = nextTWord){
                nextTword = tWord->nextWord;
                free(tWord->word);
        }
    }
    free(t);

    return;
}



//comparator function for qsort:
int comparator(const void *p, const void *q)
{
    struct Word *data1 = *(struct Word **)p;
    struct Word *data2 = *(struct Word **)q;
    //int n1 = data1->freq;
    //int n2 = data2->freq;
    return (data1->freq - data2->freq); //for descending order
}


/******************************* MAIN FUNCTION ***************************************/


int main(int argc, char* argv[])
{
	char buf[MAX_LINE], *temp;
	int  i, in_word = OUTSIDE;
	char str[MAX_WORD];
  
  
  //new addition to code:
  int fcheck; //integer receiving return value of lookupWord: checks if realloc fails
  int n;
  WT WordCounter = CreateWT();
  InitializeStore(WordCounter);

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


  //new addition to code:
  if(argc > 1){
    n = atoi(argv[1]);
  }else{
    n = WordCounter->numWords;
  }

  qsort(WordCounter->storeTable, n, sizeof(struct Word*), comparator);
  struct Word* w_temp;
  for(int i = 0; i < n; i++){
      w_temp = WordCounter->storeTable[i];
      printf("%s %d\n", w_temp->word, w_temp->freq);
  }

    return 0;
}
