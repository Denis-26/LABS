#include <stdio.h>
#include <stdlib.h>

extern const int det;

struct MyType{
    char name[128];
    char data[1024];
};

struct MyHeader{
    int det;
};

char *read_user_file(char*);
int empty_file(int);
int exist(int, char*);
