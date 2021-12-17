Student ID: 20190829
Name: Joon Park / 박준

*Note:
The product printed to stdout may have an extra zero at the front.
For example, the result of 123*123 will be 015129 instead of 15129.
This is because a string with twice the length of the largest input is allocated for the product.
Although it is possible to remove all zeros at the beginning, this was not done due to the possibility of a test case such as 000 * 000.

It should be noted that the performance is not very high since the Gradeschool() and Karatsuba() functions call strlen() and calloc()
very frequently. 

1. Dealing with numbers of digits that are not even:

Pre-processing on the input strings from stdin is performed before passing them as inputs to 
the Gradeschool() and Karatsuba() functions.
At the beginning, the length of the input strings (char* buff1, *buff2) from stdin are determined (int n, m respectively).
It n > m, then n is incremented and turned into an even number while m is made equal to n. 
Therefore, n and m become even and equal.
Two new strings (char *a, *b) with equal lengths n (remember n ==m ) are allocated with calloc. 
The original elements of buff1 and buff2 are copied to the new strings and the remaining entries are filled with ZERO. 
This effectively multiplies 10 to both inputs until both lengths are even and equal. 
The same is done when m > n or when m == n.
The number of zeros added to the inputs is recorded in a_tens and b_tens.
The number of zeros a_tens + b_tens is removed from the final product before printing to stdout.

In this way, the Karatsuba(char *a, char *b) function can receive two strings of even and equal lengths.

2. How to handle carries:
A pointer (char *carry) is used to update the carry while adding the digits.
When multiplying two digits (digitMult()) or adding two digits (digitAdd()), a pointer *carry is passed as an
input and the value of the sum or product divided by ten is passed to the pointer.

3. Functions:
*It should be noted that the approach taken is to operate on characters instead of integers.
All arrays used are character strings and thus all digits are characters.
The numerical value of a character d is found by d - '0'.


char digitAdd(char a, char b, char c, char* carry);
-> adds the numerical values of characters a (a-'0'), b (b-'0'), and c (c-'0').
-> Returns the character corresponding to the remainder of the sum when divided by 10
     and stores the quotient in the pointer carry.

char digitMult(char a, char b, char* carry);
-> multiplies the numerical values of char a and b.
-> Returns the character corresponding to the remainder of the product when divided by 10
     and stores the quotient in the pointer carry.

char max(char a, char b); int max_len(int n, int m);
char min(char a, char b);
-> These functions respectively return the larger character/integer input
     and the smaller character input.

char getDigit(const char *a, int i, int a_len);
-> getDigit() is needed when iterating through two arrays simultaneously with counter index i.
-> If one array is longer than the other, index i may exceed the length of one of the arrays. 
-> getDigit() returns 0 if the index i is out of range of the input string a.

void mult(const char *a, const char b, char *atimesb, int n);
-> Uses a for loop to iterate through the digits of char* a starting from its last digit.
    Each digit of a is iteratively multiplied to character b.
-> The result of each multiplication used to fill the input string atimesb from right to left.
-> (atimesb is the partial product of number a multiplied by a digit of b)
-> Two carries are used: a carry (char* c_prev) from multiplication of a[i] and b  
    and a carry (char* carry) from addition atimesb[j]+c_prev+carry when filling atimesb 
-> The digits of atimesb are initially all zero. atimesb is filled by iteratively
    performing atimesb[j] = atimesb[j]+ c_prev + carry.

void addPp(char *p, const char *atimesbj, int j, int p_len);
-> takes pre-existing string p with entries initialized to '0'. 
-> takes pre-existing string atimesbj which is the partial product a * b[j] 
-> iterates through p and atimesbj and adds partial product atimesbj to p:
    for i=j to i=length of p: p[i] = atimesbj[i-j] + (carries from addition)

char* add(char *a, char *b); void subtract(char *a, char *b);
-> Performs ordinary addition and subtraction of two strings a and b
-> Iterate through inputs a and b from left to right, adding/subtracting each digit along with
    the necessary carries

char* Gradeschool(const char *a, const char *b);
-> Allocates memory for product char* p (returns p in the end) and partial product atimesb
-> Iterates through digits of b and iteratively performs mult(a, b, atimesb, n) and addPp(p, atimesb, j, p_len)
     for all digits of b, updating atimesb while adding atimesb to p.

char* Karatsuba(const char *a, const char *b);
-> Performs recursive Karatsuba calls:
-> **Splits a into a1 and a0 by allocating two new arrays of half the length of a, 
    using memcpy to copy first and second halves of a to a1 and a0.
-> **The same is done for b.

4. Collaborators: 

I have not collaborated with anyone on this project except for members of the course Classum page.