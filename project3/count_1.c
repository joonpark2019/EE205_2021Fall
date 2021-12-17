#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "count.h"
#include "dynarray.h"

#define UNIT_ARRAY_SIZE 1024
#define MAX_LINE 4096
#define MAX_WORD 1024
#define WORD_ARRAY_SIZE 100000

enum {HASH_MULTIPLIER = 65599};
enum {OUTSIDE, INSIDE};

struct Word {
  char *word;                // actual word
    int freq;              // frequency found in stdin stream
    int index;            //the index is the  word
  struct Word *nextWord; //pointer to next Node for Id hash table
};

struct WordTable {
    struct Word *storageTable[UNIT_ARRAY_SIZE];
    struct Word *lookupTable[UNIT_ARRAY_SIZE];
    int lookupSize;
    int numWords; //number of distinct words encountered so far
}

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

WT CreateWT(void)
{
  /*Declare DB and assign memory 
  (initialize all entries of hash table to NULL) using calloc*/
  WT t;
  
  t = (WT) calloc(1, sizeof(struct WordTable));
  if (t == NULL) {
    fprintf(stderr, "Can't allocate a memory for WordTable\n");
    return NULL;
  }
  t->lookupSize = UNIT_ARRAY_SIZE; // start with 1024 buckets
  t->numWords = 0;

  return t;
}

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

struct Word makeWord(char *str, int word_count){
  struct Word *w;
  w = (struct Word*)calloc(1, sizeof(Word));
  w->word = str;
  w->freq = 1;
  w->index = word_count;
  if(w == NULL){
    free(w);
    return NULL;
  }
  return w;
}

//returns the index, which is the iteration when the distinct word was found, in the dynarray if found
int searchWord(WT t, const char *str){
    struct Word *w;
    int h = hashfunction(str, t->lookupSize);
    for (w = t->lookupTable[h]; w!= NULL; w = w->nextWord){
        if(strcmp(w->word, str) == 0){
            (w->freq) += 1;
            return w->index;
        }
    }
    return 0;
}


//comparator function for quicksort:
int comparator(const void *p, const void *q) 
{
    int n1 = ((struct Word *)p)->freq;
    int n2 = ((struct Word *)q)->freq; 
    return (n2 – n1); //for descending order
}

/*****************************************************************************/

int main(int argc, char* argv[])
{
	char buf[MAX_LINE], *temp;
	int  i, in_word = OUTSIDE;
	char str[MAX_WORD];

    DynArray_T WordArray = DynArray_new(WORD_ARRAY_SIZE);
    //number of words to print out:
    int n = atoi(argv[1]);

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
					printf("%s\n", str);
				}
				in_word = OUTSIDE;
			}
		}
	}
	if (in_word == INSIDE) {
		str[i] = 0;
		printf("%s\n", str);
	}


  

	return 0;
}
