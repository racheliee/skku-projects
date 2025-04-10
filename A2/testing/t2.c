
int foo(int x) {
    int a = x + 1;
    return a;
}

int bar(int x) {
    int b = x + 2;
    return b;
}

int main() {
    int a = 1  << 0;
    int b = 1 << 1;
    int c = 1 << 31;
    int d = 1 << 30;
    int e = 3 << 30;
    int f = foo(a);
    int g = bar(b);


    printf("%d", a);
    printf("%d", b);
    printf("%d", c);
    printf("%d", d);
    printf("%d", e);
    printf("%d", f);
    printf("%d", g);
    

    return 0;
}