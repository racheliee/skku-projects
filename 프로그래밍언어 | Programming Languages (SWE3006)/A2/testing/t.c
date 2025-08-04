

int sum();      
double average();      

int sum(int x, int y) {
    int temp;
    temp = x + y;
    return temp;
}

double average(int x, int y) {
    double result;
    result = (x + y) / 2.0;
    return result;
}

void showResult(int val) {
    printf("%d", val);
}

int main() {
    int a = 1;
    int b = -3;
    int c;
    double d;

    a + b;
    c = a + b * 3 ^ 2 & 7 | 4 << 1 >> 1;
    d = average(a, b);

    printf("%d", c);
    printf("%f", d);

    double x = 9.90319283 / 1.19283;

    printf("%f", x);

    float y = 9.90319283 / 1.19283;
    printf("%f", y);



    return 0;
}
