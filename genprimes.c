#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <string.h>

int main(int argc, char* argv[]) {

    int N = 1;
    int t = 1;
    FILE* fp;
    char filename[100] = "";
    double tstart = 0.0, ttaken= 0.0;    
    
    if(argc !=3){
	printf("usage: ./genprime N t\n");
	printf("N: the upper bound of the range [2,N]\n");
	printf("t: positive number less than 100\n");
	exit(1);
    }

    N = (unsigned int) atoi(argv[1]);
    t = (unsigned int) atoi(argv[2]);
  
    int* list = (int*) calloc(N+2, sizeof(int));
    
    N++;   

    tstart = omp_get_wtime();
#pragma omp parallel num_threads(t)
{
    for(int i=2; i*i <= N; i++){
        if(list[i]!=-1){
           #pragma omp for
           for(int j = i*i; j <= N; j+=i){
              list[j] = -1;	        
           }
        }
    }
}

    ttaken = omp_get_wtime() - tstart;
    

    strcpy(filename, argv[1]);
    strcat(filename, ".txt");
    
    if(! (fp = fopen(filename, "w+t"))){
	printf("Cannot create file %s\n", filename);
	exit(1);
    }

    for(int i =2; i < N; i++){
        if(list[i]!=-1) {
            fprintf(fp, "%d\n", i);
        }
    }

    fclose(fp);

    printf("Time taken for the main part: %f seconds\n", ttaken);
 
    return 0;
}
