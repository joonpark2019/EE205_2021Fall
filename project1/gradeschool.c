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
    return (sum % 10) + '0';
}

//returns the ASCI character corresponding to d from a*b = c*B + d
char digitMult(char a, char b, char* carry)
{
    char prod = (a-'0')*(b-'0');
    //printf("prod is %c\n", prod);
    *carry = ((prod) / 10) + '0';
    //printf("%c\n", *carry);
    //printf("mult %c\n", (prod - ((*carry - '0') * 10)) + '0');
    return (prod % 10) + '0';
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
    if(i < a_len && i >= 0)
        return a[i];
    else
        return '0';
}

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
        //why are both c and carry needed? - c gets carried on to the next cprev
        // c is the carry, d is the sum s and 
        d = digitMult(a[n - 1 - i], b, c); 
        //cprev is an extra variable initialized at zero which stores the carry
        atimesb[l - i - 1] = digitAdd(d, *cprev, *carry, carry); 
        *cprev = *c;
    }
    d = '0';
    //possibly error here due to index of 0
    atimesb[0] = digitAdd(d, *cprev, *carry, carry);
    printf("atimesb: %s\n", atimesb);
    assert(*carry == '0');
}

//this multiplies array a times b and returns the other
//j is passed as a parameter: b[j] is operated on
//note, the for loop in Gradeschool function INCREMENTS the j counter
void addAt(char *p, const char *atimesbj, int j, int p_len)
{
    int ab_len = strlen(atimesbj);
    //printf("ab_len is: %d\n", ab_len);
    char carry_val = '0';
    char *carry = &carry_val;

    //for (int i = p_len - j; i >= 0; i--)
    //for (int i = j; i < p_len; i++) : this was the original and p[i] = digitAdd(getDigit(atimesbj, i - j), carry, p[i], carry);
    for (int i = j; i < p_len; i++){
        p[p_len - 1 - i] = digitAdd(getDigit(atimesbj, ab_len - 1 - (i - j), ab_len), *carry, p[p_len - 1 - i], carry);
        printf("p is: %s\n", p);
    }
    assert(*carry == '0');
}

char* add(char *a, char *b)
{
    int n = strlen(a); int m = strlen(b);
    int l = max_len(n, m);
    char *sum = (char*)calloc(l+1 + 1, sizeof(char)); // extra one for the null character
    memset(sum, '0', l+1); 
    char carry_val = '0';
    char *carry = &carry_val;
    for (int i = 0; i < l; i++)
    {
        sum[l+1 - 1 -i] = digitAdd(getDigit(a, n - 1 - i, n), getDigit(b, m-1-i, m), *carry, carry);
    }
    sum[0] = *carry;
    return sum;
}

void subtract(char *a, char *b)
{
    int n = strlen(a); int m = strlen(b);
    char carry = '0';
    for (int i = 0; i < n; i++)
    {
        if ( (a[n - 1 - i] - '0') >= ( getDigit(b, m - 1 - i, m) - '0' + carry - '0' ) )
            { a[n - 1 - i] = (a[n-1 - i] - '0') - (getDigit(b,m-1 - i, m) - '0') - (carry - '0') + '0'; carry = '0'; }
        else { a[n-1 - i] = (a[n-1 - i] - '0') + (char)10 - (getDigit(b,m-1 - i,m)-'0') - (carry - '0') + '0'; carry = '1';}
    }
    assert(carry == '0');
}

//Gradeschool allocates a pointer
char* Gradeschool(const char *a, const char *b)
{

//update n and m:
    int n = strlen(a); int m = strlen(b);
    
    char *p = (char*)calloc(n+m + 1, sizeof(char));
    char *atimesbj = (char*)calloc(n+1 + 1, sizeof(char));
    memset(p, '0', n+m); memset(atimesbj, '0', n+1);
    
    //multiplication loop:
    for(int j = 0; j < m; j++){
        mult(a, b[m - 1 - j], atimesbj, n); addAt(p, atimesbj, j, n+m);
    }
    free(atimesbj);

    //remove zeros at the front:
    /*
    char *p_final = (char*)calloc(n+m + 1, sizeof(char));
    int j = 0;
    while (p[j] == '0')
        j++;
    memcpy(p_final, p + j, n+m-j);
    free(p);
    */
    
    return p;
}

int main(const int argc, const char *argv[])
{
    char buff1[1025]; char buff2[1025];

    //the string (character array ending with null character) that store arrays to be multiplied
    char *a, *b; 
    int a_tens = 0; int b_tens = 0;
    char *gradeschool;
    char *product;
    //char *sum;

    //struct timeval start_time, end_time;
    
    while( fgets(buff1, MAXLENGTH + 1, stdin) && fgets(buff2, MAXLENGTH + 1, stdin))
    {
        printf("%s\n", buff1); printf("%s\n", buff2);

        int n = (int)strlen(buff1); int m = (int)strlen(buff2);

        if (buff1[n-1] == '\n')
        {    buff1[n-1] = '\0'; n--;}
        if (buff2[m-1] == '\n')
        {    buff2[m-1] = '\0'; m--;}
        printf("n is : %d\n", n); printf("m is : %d\n", m);
        
/***************************Section that makes buff1 and buff2 even-lengthed ****** */
        if( n > m )
        {
            if ( (n%2) != 0)
            {
                n++; a_tens = 1; b_tens = n - m;
                m = n;
                a = (char*)calloc(n+1, sizeof(char));
                b = (char*)calloc(m+1, sizeof(char));
                memset(a, '0', n); memset(b, '0', m);
                memcpy(a, buff1, strlen(buff1)); memcpy(b, buff2, strlen(buff2));
            }
            //if n is even:
            else {
                //if((m%2) != 0)
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
        else if (m==n){
            if ( (m%2) != 0)
            {
                m++; n++; b_tens = 1; a_tens = 1;
                a = (char*)calloc(n+1, sizeof(char));
                b = (char*)calloc(m+1, sizeof(char));
                memset(a, '0', n); memset(b, '0', m);
                memcpy(a, buff1, strlen(buff1)); memcpy(b, buff2, strlen(buff2));
            }
            else{
                a_tens = 0; b_tens = 0;
                a = buff1; b = buff2;
            }
        }
        printf("a_tens is %d\n", a_tens); printf("b_tens is %d\n", b_tens); 
        printf("a is %s\n", a); 
        printf("b is %s\n", b); 
        product = (char*)calloc(n+m+1, sizeof(char));
/***************************************************************************** */
        //sum = add(a, b); 
        //printf("sum of a and b is %s\n", sum); 
        
        
        gradeschool = Gradeschool(a, b);
        memcpy(product, gradeschool, strlen(gradeschool) - a_tens - b_tens);
        printf("Gradeschool result is: %s \n", product);
        free(gradeschool); 
        //subtract(a, b);
        //printf("difference of a and b is %s\n", a); 
        
        
        free(product);
    }

    return 0;
}