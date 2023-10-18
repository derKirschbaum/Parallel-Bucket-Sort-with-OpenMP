#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

void swap(int* a, int* b) {

    int temp = *a;
    *a = *b;
    *b = temp;

}

int partition(int arr[], int low, int high) {

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {

        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }

    }

    swap(&arr[i + 1], &arr[high]);

    return (i + 1);
}

void quickSort(int arr[], int low, int high) {

    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }

}

int main(int argc, char* argv[]) {

    clock_t begin = clock();

    //Check arguments
    if (argc > 3 || argc < 3) {
        fprintf(stderr, "[\033[91;1mError\033[0m] Invalid arguments. This program require 2 arguments.\n[\033[94;1mUsage\033[0m] ./bucketSort <thread number> <amount of random number>\n");
        return 1;
    }

    int threadNum = atoi(argv[1]);
    int randomTime = atoi(argv[2]);

    printf("[\033[94;1mInfo\033[0m] Running with \033[33m%d\033[0m threads and \033[33m%d\033[0m integers", threadNum, randomTime);

    printf("\n[\033[94;1mInfo\033[0m] Initialize random seed");

    //Initialize random seed
    srand((unsigned)time(NULL));

    int* numArr = (int*)calloc(randomTime, sizeof(int));

    if(numArr == NULL){
        fprintf(stderr, "[\033[91;1mError\033[0m] Unable to allocate memory.");
        return 1;
    }

    printf("\n[\033[94;1mInfo\033[0m] Start random");

    //Since RAND_MAX is limited to 0x7FFF (32,767), so we need to get creative to random beyond RAND_MAX
    for (int i = 0; i < randomTime; i++) {

        int rand1 = rand();
        int rand2 = rand();
        int rand3 = rand();

        int combinedRandom = ((rand1 % 100) * 1000) + ((rand2 % 100) * 10) + (rand3 % 10);

        numArr[i] = combinedRandom;

    }

    printf("\n[\033[94;1mInfo\033[0m] Finished Random");

    double timeSpent = 0;

    int rangePerBucket = ceil(99999 / threadNum);

    int* outputArr = (int*)calloc(randomTime, sizeof(int));

    int* groupMemberCount = (int*)calloc(threadNum, sizeof(int));

    if(outputArr == NULL || groupMemberCount == NULL){
        fprintf(stderr, "[\033[91;1mError\033[0m] Unable to allocate memory.");
        return 1;
    }

    printf("\n[\033[94;1mInfo\033[0m] Start parallel section.");

    #pragma omp parallel shared(numArr, outputArr, groupMemberCount) num_threads(threadNum)
    {

        int myID = omp_get_thread_num();
        int totalThread = omp_get_num_threads();

        int beginRange = myID * rangePerBucket;
        int endRange = (myID + 1) * rangePerBucket - 1;

        int* temp = (int*)malloc(rangePerBucket * 100 * sizeof(int));

        if( temp == NULL ){

            fprintf(stderr, "[\033[91;1mError\033[0m] Thread %d is unable to allocate memory.", myID);

        }

        int memberCount = 0;

        //Put in bucket
        for (int j = 0; j < randomTime; j++)
        {
            if (numArr[j] >= beginRange && numArr[j] <= endRange) {
                temp[memberCount] = numArr[j];
                memberCount++;
            }
        }

        groupMemberCount[myID] = memberCount;

        int* myGroup = (int*)malloc(memberCount * sizeof(int));

        if( myGroup == NULL ){
            fprintf(stderr, "[\033[91;1mError\033[0m] Thread %d is unable to allocate memory.", myID);
        }

        for (int i = 0; i < memberCount; i++) {
            myGroup[i] = temp[i];
        }

        free(temp);

        //Sort
        quickSort(myGroup, 0, memberCount - 1);
        printf("\n[\033[94;1mInfo\033[0m] Thread %d of %d has finished sorting.", myID, totalThread);

        //Find the start of output array
        int startIndex = 0;
        for( int i = 0; i < myID; i++ ){
            startIndex += groupMemberCount[i];
        }

        //Combine array
        for (int k = 0; k < memberCount; k++) {

            outputArr[startIndex + k] = myGroup[k];

        }

        printf("\n[\033[94;1mInfo\033[0m] Result from thread %d has combined with output array.", myID);

        free(myGroup);
        
    }

    free(numArr);
    free(outputArr);
    free(groupMemberCount);

    clock_t end = clock();

    timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\n[\033[92;1mComplete\033[0m] Execution took \033[92;1m%f\033[0m seconds.\n", timeSpent);

    return 0;
}