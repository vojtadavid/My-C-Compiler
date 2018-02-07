void* malloc( int size );
void free( void* ptr );


void SieveOfEratosthenes(int n)
{
    // Create a boolean array "prime[0..n]" and initialize
    // all entries it as true. A value in prime[i] will
    // finally be false if i is Not a prime, else true.
//    bool prime[n+1];
//    memset(prime, true, sizeof(prime));
    int * prime=malloc((n+1)*8);
    int i=0;
    while(i<n+1){
        prime[i]=1;
        i=i+1;
    }

    int p=2;
    while(p*p<n)
//    for (int p=2; p*p<=n; p++)
    {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == 1)
        {
            // Update all multiples of p
            int i=p*2;
            while(i<n){
                prime[i]=0;
                i=i+p;
            }
//            for (int i=p*2; i<=n; i += p)
//                prime[i] = false;
        }
        p=p+1;
    }

    // Print all prime numbers
    p=2;
    while(p<n){
        if(prime[p]==1){
            printf("%d, ",p);
        }
        p=p+1;
    }
    printf("\n");
//    for (int p=2; p<=n; p++)
//        if (prime[p])
//            cout << p << " ";

    free(prime);
    return;
}


int main(){
    int n = 50;
    printf("prime numbers smaller than %d\n",n);
    printf("Correct solution:\n2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,\n");
    SieveOfEratosthenes(n);


    return 0;
}