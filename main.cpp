#include <stdio.h>
#include <math.h>

typedef struct node{
    double x;
    int len;

    char s[1];
}Node;

const char *s = "Nihao";
int main(){
    int off = 10;
    char buf[128+off];
    sprintf(buf, "%*.s%s", off, s);
    printf("%s\n", buf);

    return 0;
}