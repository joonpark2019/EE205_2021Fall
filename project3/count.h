#ifndef COUNT_H
#define COUNT_H

/**********************
 * EE205 Project 3 *
 **********************/
/* customer_manager.h */

/* forward type definition for DB_T */
/* "struct DB" should be defined in customer_manager1.c or
   customer_manger2.c */
typedef struct WordTable* WT;

/* create and return a WT structure */
WT CreateWT(void);

/* destory WT and its associated memory */
void DestroyWT(WT t);

/* Allocate the memory needed for the storeTable of WT*/
void InitializeStore(WT t);

/* takes a string and checks if it is already in the lookupTable.
If it is, it increments the corresponding struct Word's frequency.
Otherwise, it adds a new Word to the lookupTable and storeTable */
int lookupWord(WT t, const char *str);

#endif /* end of CUSTOMER_MANAGER_H */