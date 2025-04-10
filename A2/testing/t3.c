void userDefined2();

void userDefined2(int x) {
    float a=2.2;
    float b=4.64;
    float c = a + b;
    int d = 2 << 2;
    int e = -d;
    printf("%d",d);
    printf("%f",c);
}

int main(){


    
    userDefined2(3);
    return 0;
}