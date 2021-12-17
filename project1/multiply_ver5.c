#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>

#define MAXLENGTH 1024
#define TRUE    1
#define FALSE   0

//you need to pass a number

/************************BASIC FUNCTIONS for operations on digits, adding subtracting arrays**************************************/

//computes using character values storing actual integer values
//returns the corresponding ASCI character corresponding to d if a + b = c*B + d
//stores c from a*b = c*B + d in the carry pointer
char digitAdd(char a, char b, char c, char* carry)
{
    char sum = (a - '0') + (b - '0') + (c - '0');
    *carry = ((sum) / 10) + '0';
    //printf("%c\n", *carry);
    //printf("add %c\n", (sum - ((*carry - '0') * 10)) + '0');
    return (sum - ((*carry - '0') * 10)) + '0';
}

//returns the ASCI character corresponding to d from a*b = c*B + d
char digitMult(char a, char b, char* carry)
{
    char prod = (a-'0')*(b-'0');
    //printf("prod is %c\n", prod);
    *carry = ((prod) / 10) + '0';
    //printf("%c\n", *carry);
    //printf("mult %c\n", (prod - ((*carry - '0') * 10)) + '0');
    return (prod - ((*carry - '0') * 10)) + '0';
}

char max(char a, char b){
    return (a > b) ? a : b ;
}

int max_len(int n, int m){
    return (n > m) ? n : m ;
}

char min(char a, char b){
    return (a < b) ? a : b;
}

//may be trouble: 
char getDigit(const char *a, int i, int a_len){
    if(i < a_len)
        return a[i];
    else
        return '0';
}


//normal add and subtract are used in Karatsuba
char* add(char *a, char *b)
{
    int n = strlen(a); int m = strlen(b);
    int l = max(n, m);
    char *sum = (char*)calloc(l+1, sizeof(char)); // extra one for the null character
    memset(sum, '0', l);
    char carry_val = '0';
    char *carry = &carry_val;
    for (int i = 0; i < l+1; i++)
    {
        sum[i] = digitAdd(getDigit(a, i, n), getDigit(b, i, m), *carry, carry);
    }
    sum[l] = *carry;
    return sum;
}

void subtract(char *a, char *b)
{
    int n = strlen(a); int m = strlen(b);
    char carry = '0';
    for (int i = 0; i < n; i++)
    {
        if ( (a[i] - '0') >= ( getDigit(b, i, m) - '0' + carry - '0' ) )
            { a[i] = (a[i] - '0') - (getDigit(b,i, m) - '0') - (carry - '0') + '0'; carry = '0'; }
        else { a[i] = (a[i] - '0') + (char)10 - (getDigit(b,i,m)-'0') - (carry - '0') + '0'; carry = '1';}
    }
    assert(carry == '0');
}

/*
void split(char *input, char *arr1, char *arr2, int len)
{
    int k = len/2;
    for(int i = 0; i < k; i++) arr1[i] = input[i];
    for(int i = 0; i < len - k; i++) arr2[i] = input[k + i];
}
*/
/*****************************************************************************/

//a is a string so it is a character array while b is a character
//the function mult performs multiplication of array a with the character element b

void mult(const char *a, const char b, char *atimesb, int n) //n is the length of a
{
    //assert( strlen(atimesb) == n+1);
    char c_val = '0'; char cprev_val = '0'; char carry_val = '0';
    char *c = &c_val; char *cprev = &cprev_val;
    char *carry = &carry_val;
    char d = '0';

    for (int i = 0; i < n; i++)
    {
        //why are both c and carry needed? - c gets carried on to the next cprev
        // c is the carry, d is the sum s and 
        d = digitMult(a[i], b, c); 
        //cprev is an extra variable initialized at zero which stores the carry
        atimesb[i] = digitAdd(d, *cprev, *carry, carry); 
        //printf("assign %c\n", atimesb[i]);
        *cprev = *c;
        //sum = d + cprev + carry
        //carry = sum/B
        //atimesb[i] = sum - carry*B
    }
    d = '0';
    atimesb[n] = digitAdd(d, *cprev, *carry, carry);
    //printf("atimesb: %s\n", atimesb);
    assert(*carry == '0');
}

//this multiplies array a times b and returns the other
//j is passed as a parameter: b[j] is operated on
//note, the for loop in Gradeschool function INCREMENTS the j counter
void addAt(char *p, const char *atimesbj, int j, int p_len)
{
    int ab_len = strlen(atimesbj);
    char carry_val = '0';
    char *carry = &carry_val;
    //printf("j of bj is %d\n", j);
    //printf("ab_len is %d\n", ab_len);
    //for (int i = p_len - j; i >= 0; i--)
    //for (int i = j; i < p_len; i++) : this was the original and p[i] = digitAdd(getDigit(atimesbj, i - j), carry, p[i], carry);
    for (int i = j; i < p_len; i++)
        p[i] = digitAdd(getDigit(atimesbj, i - j, ab_len), *carry, p[i], carry);
        //p[p_len - 2] = digitAdd(getDigit(atimesbj, ab_len - j - 2), carry, p[p_len - 2], carry);
    assert(*carry == '0');
}


//current problem: extra zeros --> how to resolve??

void make_len_even(char **a, char **b, int *a_ten, int *b_ten){
    int n = strlen(*a); int m = strlen(*b);
    printf("n is %d\n",n); printf("n is %d\n",m);
    int length;
    char* a_temp; char* b_temp;
    if (n > m){
            length = n;
            if(length%2 != 0){
                length++;
                a_temp = (char*)calloc(length+1, sizeof(char));
                b_temp = (char*)calloc(length+1, sizeof(char));
                memset(a_temp, '0', length); memset(b_temp, '0', length);
                *a_ten = 1; *b_ten = length - m;
                
                for (int i = 0; i < n; i++)
                {
                    a_temp[length - 1 - i] = (*a)[i];
                    printf("%c\n", (*a)[i]);
                }
                for (int i = 0; i < m; i++)
                {
                    b_temp[length - 1 - i] = (*b)[i];
                    printf("%c\n", (*b)[i]);
                }
                *a = a_temp; *b = b_temp;
            }
            else{
                a_temp = (char*)calloc(length+1, sizeof(char));
                b_temp = (char*)calloc(length+1, sizeof(char));
                memset(a_temp, '0', length); memset(b_temp, '0', length);
                *a_ten = 0; *b_ten = length - m;
                
                for (int i = 0; i < n; i++)
                    a_temp[length - 1 - i] = (*a)[i];
                for (int i = 0; i < m; i++)
                    b_temp[length - 1 - i] = (*b)[i];
                *a = a_temp; *b = b_temp;
            }
        }
        else if (m >= n){
            length = m;
            if((length%2) != 0){
                length++;
                a_temp = (char*)calloc(length+1, sizeof(char));
                b_temp = (char*)calloc(length+1, sizeof(char));
                memset(a_temp, '0', length); memset(b_temp, '0', length);
                *b_ten = 1; *a_ten = length - n;
                for (int i = 0; i < m; i++)
                    b_temp[length - 1 - i] = (*b)[i];
                for (int i = 0; i < n; i++)
                    a_temp[length - 1 - i] = (*a)[i];
                *a = a_temp; *b = b_temp;
                
            }
            else{
                a_temp = (char*)calloc(length+1, sizeof(char));
                b_temp = (char*)calloc(length+1, sizeof(char));
                memset(a_temp, '0', length); memset(b_temp, '0', length);
                *b_ten = 0; *a_ten = length - n;
                for (int i = 0; i < m; i++)
                    b_temp[length - 1 - i] = (*b)[i];
                for (int i = 0; i < n; i++)
                    a_temp[length - 1 - i] = (*a)[i];
                *a = a_temp; *b = b_temp;
            }
        }
    return;
}

//Gradeschool allocates a pointer
char* Gradeschool(const char *a, const char *b)
{
    //keep track of how many zeros were added
    int a_tens = 0; int b_tens = 0;
    int *a_ten = &a_tens; int *b_ten = &b_tens; 
    int n = strlen(a); int m = strlen(b);
    int length = max_len(n, m);
    char *a_new = (char*)calloc(length+1 + 1, sizeof(char));
    char *b_new = (char*)calloc(length+1 + 1, sizeof(char));

    char *a_temp = a_new; char *b_temp = b_new;
    char **a_pointer = &a_new; char **b_pointer = &b_new;

    memcpy(a_new, a, (size_t)n); memcpy(b_new, b, (size_t)m);
    printf("a_new is %s\n", a_new); printf("b_new is %s\n", b_new);
    make_len_even(a_pointer, b_pointer, a_ten, b_ten);
    free(a_temp); free(b_temp);
    printf("a_ten is %d", *a_ten); printf("b_ten is %d", *b_ten);
    printf("a_new is %s\n", a_new); printf("b_new is %s\n", b_new);

//update n and m:
    n = strlen(a_new); m = strlen(b_new);
    
    char *p = (char*)calloc(n+m + 1, sizeof(char));
    char *atimesbj = (char*)calloc(n+1 + 1, sizeof(char));
    memset(p, '0', n+m); memset(atimesbj, '0', n+1);
    
    //multiplication loop:
    for(int j = 0; j < m; j++){
        mult(a_new, b_new[j], atimesbj, n); addAt(p, atimesbj, j, n+m);
    }
    free(atimesbj);

    //removing all zeros multiplied:
    int len_final = strlen(p);
    char*p_temp = (char*)calloc(len_final+1, sizeof(char));
    for (int i = 0; i < len_final - *a_ten - *b_ten; i++)
    {
        p_temp[i] = p[*a_ten + *b_ten + i];
    }
    free(p); p = p_temp;

    //remove zeros at the front:
    char *p_final = (char*)calloc(len_final + 1, sizeof(char));
    int j = 0;
    len_final = strlen(p);
    while (p[len_final - 1 - j] == '0')
        j++;
    for(int i = 0; i < n + m; i++)
    {
        p_final[len_final - 1 - i - j] = p[len_final - 1 - j - i];
    }
    free(p);
    
    return p_final;
}


//to include the null character or not????

//Karatsuba takes a pointer as an input to store the product
//the problem is to store the product in a string --> how to make the 

//each function call creates a temporary pointer which stores the multiplication result
//problem: the product continuously grows

//receives a pointer and returns a pointer
//n should equal m in this case
char* Karatsuba(char *a, char *b)
{
    int n = strlen(a); int m = strlen(b);
    if (n <= 4 || m <= 4)
    {
        char *p_temp;
        char*a_temp = (char*)calloc(n+1, sizeof(char));
        char *a_p = a_temp;
        char*b_temp = (char*)calloc(m+1, sizeof(char));
        char *b_p = b_temp;
        for (int i = 0; i < n; i++)
            a_temp[i] = a[n - 1 - i];
        for (int i = 0; i < m; i++)
            b_temp[i] = b[m - 1 - i];
        printf("base case!: %s times %s is %s\n", a_temp, b_temp, Gradeschool(a_temp, b_temp));
        p_temp = Gradeschool(a_temp, b_temp);
        free(a_p); free(b_p);
        return p_temp;
    }



    int a_tens = 0; int b_tens = 0;
    int *a_ten = &a_tens; int *b_ten = &b_tens; 
    
    int length = max_len(n, m);
    char *a_new = (char*)calloc(length+1 + 1, sizeof(char));
    char *b_new = (char*)calloc(length+1 + 1, sizeof(char));

    char *a_temp = a_new; char *b_temp = b_new;
    char **a_pointer = &a_new; char **b_pointer = &b_new;

    memcpy(a_new, a, (size_t)n); memcpy(b_new, b, (size_t)m);
    make_len_even(a_pointer, b_pointer, a_ten, b_ten);
    free(a_temp); free(b_temp);
    //printf("a_ten is %d", *a_ten); printf("b_ten is %d", *b_ten);
    printf("a_new is %s\n", a_new); printf("b_new is %s\n", b_new);
    //update n and m
    n = strlen(a_new); m = strlen(b_new);


    char *p = (char*)calloc(n + m + 1, sizeof(char));
    //char *p_final = (char*)calloc(n+m + 1, sizeof(char));

    int k = n/2;

    char *a0 = calloc((n/2) + 1, sizeof(char)); char *a1 = calloc((n/2) + 1, sizeof(char));
    char *b0 = calloc((m/2) + 1, sizeof(char)); char *b1 = calloc((m/2) + 1, sizeof(char));
    memset(a0, '0', n/2); memset(a1, '0', n/2); memset(b0, '0', m/2); memset(b1, '0', m/2);
    //split(a_new, a0, a1, n); split(b_new, b0, b1, m);
    memcpy(a0, a_new, n/2); memcpy(a1, a_new+(n/2), n/2);
    memcpy(b0, b_new, n/2); memcpy(b1, b_new+(m/2), n/2);
    printf("a0 is %s\n", a0); printf("a1 is %s\n", a1);
    printf("b0 is %s\n", b0); printf("b1 is %s\n", b1);

    printf("recurs. karatsuba call 1\n");
    char *p2 = Karatsuba(a1, b1);
    printf("recurs. karatsuba call 2\n");
    char *p0 = Karatsuba(a0, b0);
    char *sum1 = add(a0, a1); char*sum2 = add(b0, b1);
    printf("sum1 is %s and sum2 is %s\n", sum1, sum2);
    printf("recurs. karatsuba call 3\n");
    char *p1 = Karatsuba(sum1, sum2);
    printf("p2 is %s\n", p2); printf("p0 is %s\n", p0);
    printf("p1 is %s\n", p1);
    
    for (int i = 0; i < 2*k; i++) p[i] = p0[i];
    for (int i = 2*k; i < n+m; i++) p[i] = p2[i - 2*k];
    subtract(p1, p0); subtract(p1, p2); addAt(p, p1, k, n+m);

    free(a0); free(a1); free(b0); free(b1);
    free(p2); free(p1); free(p0); free(sum1); free(sum2);

    //removing all zeros multiplied:
    int len_final = strlen(p);
    char*p_temp = (char*)calloc(len_final+1, sizeof(char));
    for (int i = 0; i < len_final - *a_ten - *b_ten; i++)
    {
        p_temp[i] = p[*a_ten + *b_ten + i];
    }
    free(p); p = p_temp;
    
    return p;
}


int main(const int argc, const char *argv[])
{
    char buff1[1025]; char buff2[1025];

    //the string (character array ending with null character) that store arrays to be multiplied
    char *a, *b; 
    //int length, a_ten, b_ten;
    //char a_is_odd, b_is_odd;
    //char *gradeschool;
    char *karatsuba;
    //char *product;

    //struct timeval start_time, end_time;
    
    while( fgets(buff1, MAXLENGTH + 1, stdin) && fgets(buff2, MAXLENGTH + 1, stdin))
    {
        printf("%s\n", buff1); printf("%s\n", buff2);

        int n = (int)strlen(buff1); int m = (int)strlen(buff2);
        if (buff1[n-1] == '\n')
        {    buff1[n-1] = '\0'; n--;}
        if (buff2[m-1] == '\n')
        {    buff2[m-1] = '\0'; m--;}
        
        a = (char*)calloc(n+1, sizeof(char));
        b = (char*)calloc(m+1, sizeof(char));
        memcpy(a, buff1, n); memcpy(b, buff2, m);

        /*if the lengths of a and b are not even, then an extra 0 is added at the 
        beginning of the array to make the length even
        . NOTE: a and b are not strings but are character arrays */
        
       
        karatsuba = Karatsuba(a, b);
        printf("karatsuba result is: %s ", karatsuba);
        
        /*karatsuba = Karatsuba(a, b, n, m);
        int len_final = strlen(karatsuba);
        product = (char*)calloc(len_final+1, sizeof(char));
        for (int i = 0; i < len_final - a_ten - b_ten; i++)
        {
            product[i] = karatsuba[len_final - 1 - i];
        }
        printf("karatsuba result is: %s\n ", product);
        free(product);
        */

        /*
        gettimeofday(&start_time, NULL);
        
        for (int i = 0; i < 1000; i++)
        {
            karatsuba = Karatsuba(a, b, n, m);
            free(karatsuba);
        }
        
        gettimeofday(&end_time, NULL);

        karatsuba = Karatsuba(a, b, n, m);
        printf("%s ", karatsuba);
        printf("Time taken by karatsuba: %ld micro seconds\n",
        ((end_time.tv_sec * 1000000 + end_time.tv_usec) -
        (start_time.tv_sec * 1000000 + start_time.tv_usec)));
        free(karatsuba);
        */

     
        free(karatsuba);
    }

    return 0;
}