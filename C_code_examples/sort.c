void* malloc( int size );
void free( void* ptr );
int time(int arg);
void srand( unsigned seed );
int rand();

int modulo(int a,int b){
    return a-((a/b)*b);
}


int bubblesort(int * array, int N){
    int i=0;
    while (i < N - 1) {
        int j = 0;
//        printf("%d: ", i);
        while (j < N - 1) {
            if (array[j + 1] < array[j]) {
                int tmp = array[j + 1];
                array[j + 1] = array[j];
                array[j] = tmp;
            }
//            printf("%d ", j);
            j = j + 1;
        }
//        printf("\n");
        i = i + 1;
    }
    return 0;
}

int loadRandomArray(int * array, int N){
    int i=0;
    srand(time(0));
    while(i<N){
        array[i]=modulo(rand(),50);
        i=i+1;
    }
    return 0;
}

int printArray(int * array, int N){
    int i=0;
    while(i<N){
        printf("%d ",array[i]);
        i=i+1;
    }
    printf("\n");
    return 0;
}

int main(){
    int N=10;
    int * array=malloc(8*N);
    printf("Random array of Numbers\n");
    loadRandomArray(array,N);
    printArray(array,N);
    
    printf("Sorted array:\n");
    bubblesort(array,N);
    printArray(array,N);



    free(array);
    return 0;
}