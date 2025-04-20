#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

void parse_input(char *)

void modify_value(int *p) {
    *p = 100;
}

int main() {
    int a;

    printf("Enter an integer: ");
    scanf("%d", &a);

    printf("It is currently %d\n", a);

    modify_value(&a);

    printf("Now it is %d\n", a);

    return 0;
}
