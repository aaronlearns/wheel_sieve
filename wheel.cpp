/* Sieve of Pritchard in C++ */
/* original unoptimized version as presented in https://dl.acm.org/doi/10.1145/358527.358540 */
/* N >= 2 */
/* DELETION_METHOD == 1: original up then down scans */
/* DELETION_METHOD == 2: maximal up scan then down scan */

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define N 2000000000
#define DELETION_METHOD 1

static uint32_t s[N+2]; /* in 1..N+2 */
uint32_t length, maxS;

#define next(w) s[w]
#define prev(w) s[w-1]

#define Insert(w) {\
    next(maxS) = w;  prev(w) = maxS;  maxS = w; /* Insert w into W; */\
}

void Extend (uint32_t &length, uint32_t n) {
    uint32_t w, x;
    w = 1;
    x = length+1;
    while (x <= n) {
        Insert(x);
        w = next(w);
        x = length + w;
    }
    length = n;
}

#if DELETION_METHOD == 1
void Delete (uint32_t p) {
    uint32_t f, pf, temp1, temp2;
    f = p;
    while (p*f <= length) f = next(f);
    while (f > 1) {
        f = prev(f);
        pf = p*f;
        temp1 = prev(pf); temp2 = next(pf); next(temp1) = temp2; prev(temp2) = temp1; /* Remove p*f from W; */
    }
}
#elif DELETION_METHOD==2
void Delete (uint32_t p) {
    uint32_t p2, f, pf, fbound, temp1, temp2;
    p2 = p*p;
    f = p;
    while (p2*f <= length) f = next(f);
    fbound = f;
    pf = p*f;
    while (pf <= length) {
        temp1 = prev(pf); temp2 = next(pf); next(temp1) = temp2; prev(temp2) = temp1; /* Remove p*f from W; */
        f = next(f);
        pf = p*f;
    }
    f = fbound;
    while (f > 1) {
        f = prev(f);
        pf = p*f;
        temp1 = prev(pf); temp2 = next(pf); next(temp1) = temp2; prev(temp2) = temp1; /* Remove p*f from W; */
    }
}
#endif

int main (int argc, char *argv[]) {
    uint32_t k, p;
    uint32_t nrPrimes;
    uint32_t temp;
    bool printPrimes = false;
    if (argc == 2 && strcmp(argv[1], "-p") == 0) printPrimes = true;
    k = 1;
    maxS = 1; /* W = {1}; */
    length = 2;
    p = 3;
    nrPrimes = 1; /* Pr = {2}; */
    /* invariant: p = p_(k+1) and W = W_k inter {1,...,N} and length = min(P_k,N) and Pr = the primes up to p_k */
    while (p*p <= N) {
         if (length < N) {
            temp = p*length;
            if (N < temp) temp = N;
            Extend (length, temp); /* Extend W,length to minimum of p*length,N; */
            if (length == N) Insert (N+2); /* sentinel */
        }
        Delete(p); 
        /* Insert p into Pr; */
        k++; 
        p = next(1); /* p = next(W, 1); */
    }
    if (length < N) {
        Extend (length, N);
        Insert (N+2); /* sentinel */
    }
    printf("N=%d\n",N);
    if (printPrimes) printf("%d\n", 2);
    p = 3;
    while (p <= N) {
        if (printPrimes) printf("%d\n", p);
        nrPrimes++;
        p = next(p);
    }
    printf("%d primes found\n", nrPrimes);
}