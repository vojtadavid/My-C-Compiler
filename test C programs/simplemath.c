//
// Created by vojta on 6/11/17.
//

double acos( double arg );

int main(){
    printf("Example of simple math and variables.")
    int i=-7;
    printf("%d %d\n",i,-i);
    double d=-1.1;
    printf("%f %f\n",d,-d);
    int j=2;
    printf("-3=%d\n",i/j);
    printf("-3=%d\n",-i/-j);

    printf("module 10%3=%d\n",10%3);


    printf("acos(-1) = %f\n", acos(-1.0));
    printf("acos(0.0) = %f 2*acos(0.0) = %f\n", acos(0.0), 2*acos(0.0));
    printf("acos(0.5) = %f 3*acos(0.5) = %f\n", acos(0.5), 3*acos(0.5));
    printf("acos(1) = %f\n", acos(1.0));


    return 0;

}