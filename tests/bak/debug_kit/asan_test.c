#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = (int *)malloc(10 * sizeof(int));
    arr[10] = 0;  // 越界访问
    free(arr);
    return 0;
}