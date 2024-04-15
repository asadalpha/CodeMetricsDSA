#include <stdio.h>

void function2(int x) {
    if (x > 0) {
        printf("x is positive\n");
    } else if (x < 0) {
        printf("x is negative\n");
    } else {
        printf("x is zero\n");
    }
}

int main() {
    int a = 10;
    for (int i = 0; i < a; i++) {
        if (i % 2 == 0) {
            printf("%d is even\n", i);
        } else {
            printf("%d is odd\n", i);
        }
    }
    function2(a);
    return 0;
}
