//
// Created by vojta on 6/11/17.
//

int fact(int n){
//    printf("fact %d\n",n);
    if(n==1){
        return 1;
    }
    return n*fact(n-1);
}

double factDouble(int n){
    if(n==1){
        return 1.0;
    }
    return n*factDouble(n-1);
}

int Fibonacci(int n) {
//    printf("f %d\n",n);
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return (Fibonacci(n - 1) + Fibonacci(n - 2));
}

int main(){
    int i=1;
    printf("Factorial\n");
    while(i<7){
        printf("(%d, %f) ",fact(i),factDouble(i));
        i=i+1;
    }
    printf("\n");
    printf("Fibonacci 0, 1, 1, 2, 3, 5, 8, 13, 21\n\t");
    i=0;
    while(i<9){
        printf("%d, ",Fibonacci(i));
        i=i+1;
    }
    printf("\n");


    return 0;
}