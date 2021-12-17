//Student ID: 20190829
//Name: Joon Park (박준)

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define MAXLENGTH 1024

//functions for operating on single digits:
char digitAdd(char a, char b, char c, char* carry);
char digitMult(char a, char b, char* carry);
char getDigit(const char *a, int i, int a_len);
//functions for comparing lengths:
char max(char a, char b); int max_len(int n, int m);
char min(char a, char b);

//functions needed by gradeschool and karatsuba algorithms:
void mult(const char *a, const char b, char *atimesb, int n);
void addPp(char *p, const char *atimesbj, int j, int p_len);
char* add(char *a, char *b); void subtract(char *a, char *b);

//Gradeschool algorithm function:
char* Gradeschool(const char *a, const char *b, int n, int m);
//Karatsuba algorithm function:
char* Karatsuba(const char *a, const char *b);

/*****************************BASIC FUNCTIONS for operations on digits and lengths**************************************/

//digitAdd computes using character values storing actual integer values
//digitAdd returns the corresponding ASCI character corresponding to d if a + b = c*B + d (B=10)
//stores c from a*b = c*B + d in the carry pointer
char digitAdd(char a, char b, char c, char* carry)
{
    char sum = (a - '0') + (b - '0') + (c - '0');
    *carry = ((sum) / 10) + '0';
    return (sum % 10) + '0';
}

//digitMult returns the ASCI character corresponding to d from a*b = c*B + d
//stores c from a*b = c*B + d in the carry pointer
char digitMult(char a, char b, char* carry)
{
    char prod = (a-'0')*(b-'0');
    *carry = ((prod) / 10) + '0';
    
    return (prod % 10) + '0';
}

//max receives two characters as inputs
//returns the character corresponding to the larger integer value
char max(char a, char b){
    return (a > b) ? a : b ;
}

//max_len receives two integers as inputs
//returns the larger integer value
int max_len(int n, int m){
    return (n > m) ? n : m ;
}

//min receives two characters as inputs
//returns the character corresponding to the smaller integer value
char min(char a, char b){
    return (a < b) ? a : b;
}

//getDigit receives a string a, an index i, and the length of the string a
//returns the character at index i if i is within the length of a
//otherwise, getDigit returns 0
char getDigit(const char *a, int i, int a_len){
    if(i < a_len && i >= 0)
        return a[i];
    else
        return '0';
}

/********************************FUNCTIONS NEEDED IN MULTIPLYING, ADDING, Subtracting TWO STRINGS**************************************/

void mult(const char *a, const char b, char *atimesb, int n) //n is the length of a
{
    //assert( strlen(atimesb) == n+1);
    char c_val = '0'; char cprev_val = '0'; char carry_val = '0';
    char *c = &c_val; char *cprev = &cprev_val;
    char *carry = &carry_val;
    char d = '0';
    int l = strlen(atimesb); 
    //counter variable:
    int i;

    for (i = 0; i < n; i++)
    {
        //for loop iteration is done from right to left while the counter is incremented:
        //therefore, the indexing of the string is (length of string - 1 - counter)
        //the same applies throughout the rest of the file
        d = digitMult(a[n - 1 - i], b, c); 
        //cprev is an extra variable initialized at zero which stores the carry
        atimesb[l - i - 1] = digitAdd(d, *cprev, *carry, carry); 
        *cprev = *c;
    }
    d = '0';
    //possibly error here due to index of 0
    atimesb[0] = digitAdd(d, *cprev, *carry, carry);
    assert(*carry == '0');
}

//addPp stands for 'add partial product'
/* Receives the partial product string atimesbj from a*b[j] 
and adds it to final product string p. Receives index j and the length of
p (p_len) as inputs */
void addPp(char *p, const char *atimesbj, int j, int p_len)
{
    int ab_len = strlen(atimesbj);
    char carry_val = '0';
    char *carry = &carry_val;
    
    /*for loop adds the elements of atimesbj to the pre-existing elements of p */
    for (int i = j; i < p_len; i++){
        p[p_len - 1 - i] = digitAdd(getDigit(atimesbj, ab_len - 1 - (i - j), ab_len), *carry, p[p_len - 1 - i], carry);

    }
    assert(*carry == '0');
}

//ordinary add function needed to add two arrays in karatsuba multiplication
//returns a new string which contains the sum of a and b: 
//if n = strlen(a) and m = strlen(b), then string returned has length n+m+1
char* add(char *a, char *b)
{
    int n = strlen(a); int m = strlen(b);
    int l = max_len(n, m);
    // extra one character (l + 1 instead of l) for possible carry over to an extra digit
    char *sum = (char*)calloc(l+1 + 1, sizeof(char)); 
    memset(sum, '0', l+1); 
    //carry pointer used to update the carry from the addition of each digit
    char carry_val = '0';
    char *carry = &carry_val;
    //iterate through both a and b and add a[i] digit and b[i] digit along with possible carry
    for (int i = 0; i < l; i++)
    {
        sum[l+1 - 1 -i] = digitAdd(getDigit(a, n - 1 - i, n), getDigit(b, m-1-i, m), *carry, carry);
    }
    sum[0] = *carry;
    return sum;
}

//ordinary subtract function needed to subtract two arrays in karatsuba multiplication
//The difference a minus b is stored in a
void subtract(char *a, char *b)
{
    int n = strlen(a); int m = strlen(b);
    char carry = '0';
    //Iteration through each element of a and subtract (b[i]+carry) from a[i]:
    for (int i = 0; i < n; i++)
    {
        //if a[i] is greater than b[i], assign '0' to carry and subtract as normal
        if ( (a[n - 1 - i] - '0') >= ( getDigit(b, m - 1 - i, m) - '0' + carry - '0' ) )
            { a[n - 1 - i] = (a[n-1 - i] - '0') - (getDigit(b,m-1 - i, m) - '0') - (carry - '0') + '0'; carry = '0'; }
        //if a[i] is smaller than b[i], a 'borrow' is performed:
        //'1' is assigned to carry so that an extra one is subtracted from the subtraction of the next digit
        else { a[n-1 - i] = (a[n-1 - i] - '0') + (char)10 - (getDigit(b,m-1 - i,m)-'0') - (carry - '0') + '0'; carry = '1';}
    }
    assert(carry == '0');
}


/******************************** Gradeschool, Karatsuba: **************************************/

//Gradeschool() is the implementation of the gradeschool algorithm
//returns a string p which contains the product of string inputs char* a and char*b
char* Gradeschool(const char *a, const char *b, int n, int m)
{
    //int n = strlen(a); int m = strlen(b);
    
    //p is the string that is returned
    char *p = (char*)calloc(n+m + 1, sizeof(char));
    //atimesbj is the array result of (array a) * (element b[j])
    char *atimesbj = (char*)calloc(n+1 + 1, sizeof(char));
    memset(p, '0', n+m); memset(atimesbj, '0', n+1);
    
    //multiplication loop:
    for(int j = 0; j < m; j++){
    //a * b[j] is performed for all elements of array b
    //for each iteration, atimesbj is added to p shifted from the right by j
        mult(a, b[m - 1 - j], atimesbj, n); addPp(p, atimesbj, j, n+m);
    }
    free(atimesbj);

//remove any zeros at the front:
//ex. 1234*1234 -> 1522756 instead of 01522756
    char *p_final = (char*)calloc(n+m + 1, sizeof(char));
    int j = 0;
    if(p[0] == '0')
        j++;
    memcpy(p_final, p + j, n+m-j);
    free(p);

    return p_final;
}


char* Karatsuba(const char *a, const char *b)
{
    //n should equal m
    int n = strlen(a); int m = strlen(b);
    char *p, *a_new, *b_new;
    int a_tens = 0; int b_tens = 0;

    /*recursive base case: when the inputs have lengths 8 or less,
    the normal gradeschool algorithm is used*/
    if (n <= 8 || m <= 8)
    {
        p = Gradeschool(a, b, n, m);
        return p;
    }

    /*Karatsubs() requires the inputs to have equal and even lengths.
    It is guaranteed from the pre-processing of the inputs before the
    Karatsuba() call that n == m. 
    If n is not even, n and m are incremented and new strings are allocated
    with the lengths made even by adding an extra zero (effect of multiplying 10.
    The mumber of zeros added is removed from the final produce at the end */
     if ((n%2) != 0)
     {
        //the number of zeros added is recorded in a_tens, b_tens
        n++; m++; a_tens = 1; b_tens = 1; 
        a_new = (char*)calloc(n+1, sizeof(char));
        b_new = (char*)calloc(m+1, sizeof(char));
        memset(a_new, '0', n); memset(b_new, '0', m);
        memcpy(a_new, a, n-1); memcpy(b_new, b, m-1);
     }
     else{ //if n and m are already even:
        a_new = (char*)calloc(n+1, sizeof(char));
        b_new = (char*)calloc(m+1, sizeof(char));
        memcpy(a_new, a, n); memcpy(b_new, b, m);
     }

    p = (char*)calloc(n + m + 1, sizeof(char));
    memset(p, '0', n+m);
    int k = n/2;

    //a = a1*10^(n/2) + a0,  b = b1*10^(n/2) + b0
    //allocating new memory for a1, a0, b1, b0:
    char *a0 = calloc(k + 1, sizeof(char)); char *a1 = calloc(k + 1, sizeof(char));
    char *b0 = calloc(k + 1, sizeof(char)); char *b1 = calloc(k + 1, sizeof(char));
    memset(a0, '0', k); memset(a1, '0', k); 
    memset(b0, '0', k); memset(b1, '0', k);

    //splitting the array in half:
    memcpy(a1, a_new, k); memcpy(a0, a_new + k, k);
    memcpy(b1, b_new, k); memcpy(b0, b_new + k, k);

    //recursive function call:
    char *p2 = Karatsuba(a1, b1);
    char *p0 = Karatsuba(a0, b0);
    char *sum1 = add(a0, a1); char*sum2 = add(b0, b1);
    char *p1 = Karatsuba(sum1, sum2);

    int l0 = strlen(p0); int l2 = strlen(p2);
    
    //assigning digits to final return value:
    for (int i = 0; i < 2*k; i++) p[n + m - 1 - i] = getDigit(p0, l0 - 1 - i, l0);
    for (int i = 0; i < l2; i++) p[n + m - (2*k) - 1 - i] = getDigit(p2, l2 - 1 - i, l2);
    subtract(p1, p0); subtract(p1, p2); 
    addPp(p, p1, k, n+m);

    free(a_new); free(b_new); free(a0); free(a1); free(b0); free(b1);
    free(p2); free(p1); free(p0); free(sum1); free(sum2);


//remove the extra zeros that were added at the end and any zeros at the front:
    char*p_final = (char*)calloc(n+m+1, sizeof(char));
    int j = 0;
    if(p[0] == '0')
        j++;
    memcpy(p_final, p+j, n+m -j - a_tens - b_tens);
    free(p);

    return p_final;
}

int main(const int argc, const char *argv[])
{
    //buffers for receiving two strings from stdin:
    char buff1[MAXLENGTH + 1]; char buff2[MAXLENGTH + 1];

    //strings that will store the pre-processed versions of the input:
    char *a, *b; 

    //pointers to results of Karatsuba and Gradeschool functions
    char *karatsuba; char *gradeschool;
    //integers that record number of zeros added to a and b respectively to make lengths even and equal:
    int a_tens = 0; int b_tens = 0;
    //pointer to string to which the digits of the final product will be copied to be printed to stdout:
    char *product;

    //start and end time:
    //struct timeval start_time, end_time;
    struct timespec start_time, end_time;
    
    while( fgets(buff1, MAXLENGTH + 1, stdin))
    {
    //error handling: if a newline is read without an integer, the next number is read
        if(*buff1 == '\n'){
            printf("Read empty line instead of number: reading next number\n");
            continue;
        }
        assert( fgets(buff2, MAXLENGTH + 1, stdin)!= NULL);
        if(*buff2 == '\n'){
            printf("Second number is an empty line: reading next number\n");
            continue;
        }
        printf("%s\n", buff1); printf("%s\n\n", buff2);

    //since input has a 
        int n = (int)strlen(buff1); int m = (int)strlen(buff2);
        if (buff1[n-1] == '\n')
        {    buff1[n-1] = '\0'; n--;}
        if (buff2[m-1] == '\n')
        {    buff2[m-1] = '\0'; m--;}

/**************Section that makes inputs even-lengthed by adding zeros****************************************** */
        if( n > m )
        {
            //if n is not even:
            if ( (n%2) != 0)
            {
                //incrememt n by one, making n even and make m equal n
                n++; a_tens = 1; b_tens = n - m;
                m = n;
                a = (char*)calloc(n+1, sizeof(char));
                b = (char*)calloc(m+1, sizeof(char));
                //memset used to fill remaining digits with zero
                memset(a, '0', n); memset(b, '0', m);
                memcpy(a, buff1, strlen(buff1)); memcpy(b, buff2, strlen(buff2));
            }
            //if n is already even:
            else {
                b_tens = n - m;
                m = n;
                a = buff1; b = (char*)calloc(m+1, sizeof(char)); memset(b, '0', m);
                memcpy(b, buff2, strlen(buff2));
            }
            
        }
        else if (m > n)
        {
            if ( (m%2) != 0)
            {
                m++; b_tens = 1; a_tens = m - n;
                n = m;
                a = (char*)calloc(n+1, sizeof(char));
                b = (char*)calloc(m+1, sizeof(char));
                memset(a, '0', n); memset(b, '0', m);
                memcpy(a, buff1, strlen(buff1)); memcpy(b, buff2, strlen(buff2));
            }
            //if m is even BUT n is not even
            else //if( (n%2)  != 0)
            {
                a_tens = m - n;
                n = m;
                b = buff2; a = (char*)calloc(n+1, sizeof(char)); memset(a, '0', m);
                memcpy(a, buff1, strlen(buff1));
            }
            //if both m and n are even
            
        }
        else if (n == m){
        //else if n == m but n and m are not even:
            if ( (m%2) != 0)
            {
                m++; n++; b_tens = 1; a_tens = 1;
                a = (char*)calloc(n+1, sizeof(char));
                b = (char*)calloc(m+1, sizeof(char));
                memset(a, '0', n); memset(b, '0', m);
                memcpy(a, buff1, strlen(buff1)); memcpy(b, buff2, strlen(buff2));
            }
            else{
            //else if n == m and n, m are both even:
                a_tens = 0; b_tens = 0;
                a = buff1; b = buff2;
            }
        }

        assert(a != NULL); assert(b != NULL);
        product = (char*)calloc(n+m+1, sizeof(char));
/***************************************************************************** */

        //running gradeschool algorithm:
        
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        for(int i = 0; i < 1000; i++)
        {
            gradeschool = Gradeschool(a, b, n, m); free(gradeschool); 
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time); 
        long duration = (end_time.tv_sec - start_time.tv_sec)*1000000000 + 
                            (end_time.tv_nsec - start_time.tv_nsec);
        
        gradeschool = Gradeschool(a, b, n, m);
        /*remove all of the extra zeros multiplied to make 
        lengths of a and b equal and even: memcpy only the number of characters
        without the extra zeros multiplied: */
        memcpy(product, gradeschool, strlen(gradeschool) - a_tens - b_tens);
        printf("%s ", product);
        printf("Gradeschool: %.3lf us\n\n", (double)duration/1000);
        
        free(gradeschool);
        //initialize all entries to NULL for karatsuba algorithm
        memset(product, '\0', n+m);

        //running karatsuba algorithm:
        //gettimeofday(&start_time, NULL);
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        for(int i = 0; i < 1000; i++)
        {
            karatsuba = Karatsuba(a, b); free(karatsuba); 
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time); 
        //gettimeofday(&end_time, NULL);
        duration = (end_time.tv_sec - start_time.tv_sec)*1000000000 + 
                    (end_time.tv_nsec - start_time.tv_nsec);
        karatsuba = Karatsuba(a, b);
        /*remove all of the extra zeros multiplied to make 
        lengths of a and b equal and even: */
        memcpy(product, karatsuba, strlen(karatsuba) - a_tens - b_tens);
        printf("%s ", product);
        printf("Karatsuba: %.3lf us\n\n", (double)duration/1000);
        
        free(karatsuba); 
        free(product);
    }

    return 0;
}