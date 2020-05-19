//
// Created by hanifa on 5/14/20.
//

#include "ThreadSorting.h"
#include <iostream>
#include <thread_db.h>
#include <fstream>
#include <vector>
#include <cstring>


using namespace std;

int n1;
string filename = "numbers.txt";
pthread_mutex_t lock;


/* Constructor */
ThreadSorting::ThreadSorting() {}

/**
 * Reads from the file
 * @param nums
 * @param len
 * @return
 */
vector<int> ThreadSorting::readFromFile(vector<int> nums, int len) {
    ifstream isObj(filename);
    if (!isObj) {
        cout << "Failed to open file" << endl;
    }

    string c;
    while (isObj >> c) {
        int current = stoi(c);
        nums.push_back(current);
    }
    isObj.close();
    return nums;
}

/**
 * Writes to the file
 * @param nums
 * @param len
 */
void ThreadSorting::writeToFile(vector<int> nums, int len) {
    ofstream osObj("numbers.txt");
    if (!osObj) {
        cout << "Issue with accessing the file" << endl;
    }

    osObj.clear();
    vector<int>::iterator it;
    for (it = nums.begin(); it != nums.end(); ++it) {
        int current = *it;
        string str = to_string(current);
        osObj << str;
        osObj << "\t";
    }
    // Close the file
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

    for (int j = low; j <= high- 1; j++) {
        //if current element is smaller than pivot, increment the low element
        //swap elements at i and j
        if (arr[j] <= pivot) {
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
    pthread_mutex_lock(&lock);

    int* pair = new int[2];
    pair = generatePair(n1);
    //int i = *(pair + 0);
    //int j = *(pair + 1);
    int i = 0;
    int j = 3;
    int len = (j - i) + 1;
    int arr[len];

    // Read values from the file
    vector<int> nums;
    nums = readFromFile(nums, len);

    // iterate through the vector
    int idx = 0;
    for (int a = i; a <= j; a++) {
        arr[idx] = nums.at(a);
        //cout << "the current number is: " << arr[idx] << endl;
        idx++;
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
    vector<int> convertedArray(arr, arr + len);
    writeToFile(convertedArray, len);
    pthread_mutex_unlock(&lock);

    // Thread manager
    // TODO: use a thread and maybe move it to the main function
    bool stop = isSorted(arr, len);
    if (stop) {
        cout << "Simulation terminated " << endl;
        exit(2);
    } else {
        void* r;
        computation(r);
    }
    return (void*)0;
}

/**
 * Checks whether the list is completely sorted
 * @return
 */
bool ThreadSorting::isSorted(int arr[], int len) {
    pthread_mutex_lock(&lock);
    if (len == 0 || len == 1) { return true; }
    for (int i = 1; i < len; i++) {
        // Unsorted pair found
        if (arr[i - 1] > arr[i]) { return false; }
    }
    pthread_mutex_unlock(&lock);
    return true;
}

/**
 * Main function for the program
 * @param n command line argument for the number of elements
 * @return
 */
int ThreadSorting::main(int n) {
    n1 = n;
    string arr[n1];
    srand(time(NULL));
    vector<int> numbersGenerated;
    for (int o = 0; o < n1; o++) {
        int number = rand() % 100 ;
        numbersGenerated.push_back(number);
    }
    writeToFile(numbersGenerated, n1);

    // Get the number of threads
    string threadsCount;
    cout << "Enter the number of threads: " << endl;
    getline(cin, threadsCount);
    int M = stoi(threadsCount);

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

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

    pthread_mutex_destroy(&lock);
    return 0;
}