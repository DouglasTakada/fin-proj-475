#include <stdio.h>
void foo(void){
    if(0){
        printf("Hello world");
    }
    boo();
    boo();
    if(0){
        ("Hello world");
    }
}

int goo(void){
    if(0){
        printf("Hello world");
    }
    foo();
    boo();
    if(0){
        printf("Hello world");
    }
    return 0;
}

int boo(){
    if(0){
        printf("Hello world");
    }
    printf("");
    if(0){
        printf("Hello world");
    }
    
    return 0;
}