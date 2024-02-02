// target.c

#include "target_program.h"

void get_list(char* a) {
    // send 함수 호출
    int result = send(4, 7);
    printf("result : %d", result);

    // 특정 동작 수행
    // ...
}

int send(int a, int b) {
    // send 함수의 실제 구현
    printf("Im real send\n");
    return a + b;
}
