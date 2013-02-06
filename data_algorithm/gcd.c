/**
 * 最大公因数 欧几里德算法
 * 2013-02-06
 */

#include <stdio.h>

unsigned int gcd(unsigned int m, unsigned int n) {
    unsigned int rem;

    while (n > 0) {
        rem = m % n;
        m = n;
        n = rem;
    }
    return m;
}

int main(int argc, char *argv[]) {
    printf("%d\n", gcd(50, 15));
    
    return 0;
}
