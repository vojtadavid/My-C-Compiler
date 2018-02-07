//
// Created by vojta on 6/10/17.
//
double fmod (double numer, double denom);
int time(int arg);
void srand( unsigned seed );
int rand();
void* malloc( int size );
void free( void* ptr );
double acos( double arg );
double sin( double arg );
double sqrt( double arg );
double  fabs( double arg );

void merge(double* arr, int l, int m, int r)
{

    int i=12;
    int j=23;
    int k=34;
    int n1 = m - l + 1;
    int n2 = r-m;


    double *L=malloc(n1*8);
    double *R=malloc(n2*8);

    i=0;
    while (i<n1){
        L[i] = arr[l + i];
//        printf("%f %f ",L[i],arr[l + i]);
        i=i+1;

    }

    i=0;
    while (i<n2){
        R[i] = arr[m + 1+ i];
//        printf("%f %f ",R[i],arr[m + 1+ i]);
        i=i+1;

    }
//    printf("\n");
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] < R[j])
        {
            arr[k] = L[i];
            i=i+1;
        }
        else
        {
            arr[k] = R[j];
            j=j+1;
        }
        k=k+1;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i=i+1;
        k=k+1;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j=j+1;
        k=k+1;
    }

    free(L);
    free(R);
    return;
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(double* arr, int l, int r)
{
//    printf("mergerSort %d %d\n",l,r);
//    int i=l;
//    while(i<r){
//        printf("%d ",arr[i]);
//        i=i+1;
//    }
//    printf("\n");
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
//        int m = l+(r-l)/2;
        int m= (l+r)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
    return;
}


int loadRandomArray(double * array, int N){
    int i=0;
    srand(time(0));
    while(i<N){
        array[i]=fmod((rand()+0.0)/1000000.0,100.0);
        i=i+1;
    }
    return 0;
}

int printArray(double * array, int N){
    int i=0;
    while(i<N){
        printf("%f ",array[i]);
        i=i+1;
    }
    printf("\n");
    return 0;
}


int main()
{

    int N=10;
    double * array=malloc(8*N);
    loadRandomArray(array,N);
    printArray(array,N);
    mergeSort(array,0,N-1);
    printArray(array,N);
    free(array);


    return 0;
}