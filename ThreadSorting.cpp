//
// Created by hanifa on 5/14/20.
//

#include "ThreadSorting.h"
#include <iostream>
#include <thread_db.h>
#include <fstream>


using namespace std;

int n1;

ThreadSorting::ThreadSorting() {
}


char& ThreadSorting::readFromFile(char *arr, int len) {
    ifstream isObj("numbers.txt");
    if (!isObj) {
        cout << "Failed to open file" << endl;
    }
    isObj.read(arr, len);
    isObj.close();
    return *arr;
}

void ThreadSorting::writeToFile(char* arr, int len) {
    ofstream osObj("numbers.txt");
    if (!osObj) {
        cout << "Issue with accessing the file" << endl;
    }
    osObj.write(arr, len);
    osObj.close();
}



/**
 * Generates a pair of numbers with the following condition:
 *  0 <= pair[0] <= pair[1] < n
 * @return pair
 */
int* ThreadSorting::generatePair(int n) {
    int* pair[2];
    int num_1 = rand() % n;
    int num_2 = rand() % n;
    if (num_1 <= num_2) {
        pair[0] = &num_1;
        pair[1] = &num_2;
    }
    else {
        pair[0] = &num_2;
        pair[1] = &num_1;
    }
    return *pair;
}

void ThreadSorting::swap(int* pointer1, int* pointer2) {
    int temp = *pointer1;
    *pointer1 = *pointer2;
    *pointer2 = temp;
}

int ThreadSorting::partition(int arr[], int low, int high) {
    int pivot = arr[high];    // pivot
    int i = (low - 1);

    for (int j = low; j <= high- 1; j++)
    {
        //if current element is smaller than pivot, increment the low element
        //swap elements at i and j
        if (arr[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/**
 * Quick sort algorithm
 * @param arr
 * @param low
 * @param high
 */
void ThreadSorting::quicksort(int arr[], int low, int high) {
    if (low < high) {
        //partition the array
        int pivot = partition(arr, low, high);
        //sort the sub arrays independently
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}


/**
 * Insertion sort algorithm
 * @param arr
 * @return
 */
int * ThreadSorting::insertsort(int* arr, int len) {
    int a, key, b;
    for (a = 1; a < len; a++) {
        key = arr[a];
        b = a - 1;
        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (b >= 0 && arr[b] > key) {
            arr[b + 1] = arr[b];
            b = b - 1;
        }
        arr[b + 1] = key;
    }
    return arr;
}

/**
 * Bubble sort algorithm
 * @param arr
 * @return
 */
int * ThreadSorting::bubblesort(int* arr, int len) {
    // TODO try to use only temp instead of using both temp and result
    int* temp = new int[len];
    std::copy(arr, arr + len, temp);
    int* result;
    result = new int [len];
    int a, b;
    for (a = 0; a < len-1; a++) {
        for (b = 0; b < len - a - 1; b++) {
            if (temp[b] > temp[b + 1]) {
                swap(&temp[b], &temp[b + 1]);
            }
        }
    }
    result = temp;
    return result;
}


/**
 *
 * @param r
 * @return
 */
void * ThreadSorting::computation(void *r) {
    // Generates a random pair of numbers ranging from 0 to n
    int* pair;
    pair = generatePair(n1);
    int i = *(pair + 0);
    int j = *(pair + 1);
    char *array = nullptr;
    int len = (j - i) + 1;
    int arr[len];

    // Read values from the file
    readFromFile(array, len);

    for (int a = 0; a < len; a++) {
        arr[a] = atoi(reinterpret_cast<const char*>(array[a]));
    }
    // Randomly select which sorting algorithm to use between
    // a) quick sort , b) insert sort, and c) bubble sort
    int sortingOption = rand() % 3;
    switch (sortingOption) {
        case 0: {
            quicksort(arr, 0, len -1);
            break;
        }
        case 1: {
            insertsort(arr, len);
            break;
        }
        case 2: {
            bubblesort(arr, len);
            break;
        }
    }
    // Write sorted values to the file

    return (void*)0;
}

/**
 * Checks whether the list is completely sorted
 * @return
 */
bool ThreadSorting::isSorted() {
    bool flag = false;
    //
    if (!flag) {

    }
    else { // Generate a new pair (i, j)
        int* pair;
        pair = generatePair(n1);
        int i = *(pair + 0);
        int j = *(pair + 1);
    }
    return true;
}

int ThreadSorting::main(int n) {
    n1 = n;
    char arr[n1];
    srand(time(NULL));
    for (int o = 0; o < n1; o++) {
        int a = rand() % 100 ;
        arr[o] = '0' + a + '\t';
    }
    writeToFile(arr, n1);


    // Get the number of threads
    string threadsCount;
    cout << "Enter the number of threads: " << endl;
    getline(cin, threadsCount);
    int M = stoi(threadsCount);

    // Start the M threads
    thread_t threads[M];
    pthread_attr_t attr;

    // Create the threads
    for (int i = 0; i < M; i++) {
        pthread_create(&threads[i], 0, computation, (void*) (long) i);
    }

    // Wait for the threads to finish
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }



    return 0;
}