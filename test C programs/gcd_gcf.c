//
// Created by vojta on 6/12/17.
//


int gcd(int x, int y) {
    if (x == 0) {
        return y;
    }

    while (y != 0) {
        // printf("gcd %d %d\n",x,y);
        if (x > y) {
            x = x - y;
        }
        else {
            y = y - x;
        }
    }

    return x;
}

int gcd_recursion(int a, int b) {
    if (b == 0) {
        return a;
    }
    else {
        return gcd(b, a%b);
    }
}



int main(){
    int x=1225;
    int y=50;

    int hcf = gcd(x, y);

    printf("Greatest common divisor of %d and %d = %d %d\n", x, y, hcf,gcd_recursion(x,y));


    return 0;
}