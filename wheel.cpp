/* Sieve of Pritchard in C++ */
/* naive, unoptimized version */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define N 1000000

#define Insert(w) {\
     s[maxS] = w;  s[w-1] = maxS;  maxS = w; /* Insert w into W; */\
}

#define Extend(length, n) {\
    uint32_t w, x;\
    w = 1;\
    x = length+1;\
    while (x <= n) {\
        Insert (x);\
        w = s[w]; /* w := next(W,w); */\
        x = length + w;\
	}\
    length = n;\
}

#define Delete(p) {\
    uint32_t w, temp1, temp2;\
    w = p;\
    while (p*w <= length)\
        w = s[w]; /* w := next(W,w); */\
	while (w > 1) {\
        w = s[w-1]; /* w := prev(W,w); */\
        temp1 = p*w; temp2 = s[temp1-1]; s[temp2] = s[temp1]; s[s[temp1]-1] = temp2; /* Remove p*w from W; */\
	}\
}

int main (int argc, char *argv[]) {
    static uint32_t s[N+2]; /* in 1..N+2 */
	uint32_t maxS;
    uint32_t k, length, p;
	uint32_t nrPrimes;
    k = 1;
    maxS = 1; /* W = {1}; */
    length = 2;
    p = 3;
    nrPrimes = 1; /* Pr = {2}; */
    /* invariant: p = p_(k+1) and W = W_k inter {1,...,N} and length = min(P_k,N) and Pr = the primes up to p_k */
    while (p*p <= N) {
        if (length < N) {
			uint32_t temp = p*length;
			if (N < temp) temp = N;
            Extend (length, temp); /* Extend W,length to minimum of p*length,N; */
			if (length == N) Insert (N+2); /* sentinel */
		}
        Delete (p); 
        /* Insert p into Pr; */
        k++; 
        p = s[1]; /* p = next(W, 1); */
	}
    if (length < N) {
        Extend (length, N);
		Insert (N+2); /* sentinel */
	}
    /* print primes */
	printf("N=%d\n",N);
	/*printf("%d\n", 2);*/
	p = 3;
	while (p <= N) {
		/*printf("%d\n", p);*/
		nrPrimes++;
		p = s[p];
	}
	printf("%d primes found\n", nrPrimes);
}