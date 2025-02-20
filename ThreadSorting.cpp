//
// Created by hanifa on 5/14/20.
//

#include "ThreadSorting.h"
#include <iostream>
#include <thread_db.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <mutex>
#include <climits>
#include <unistd.h>
#include <syscall.h>


using namespace std;


int totalCount;
int M;
string filename = "numbers.txt";
bool stop = false;
pthread_mutex_t lock1;
pthread_mutex_t lock2;

/* Constructor */
ThreadSorting::ThreadSorting() {}

/**
 * Reads elements in a given range from the file
 * @param nums
 * @param start
 * @param end
 * @return
 */
vector<int> ThreadSorting::readFromFile(vector<int> nums, int start, int end) {
    ifstream isObj(filename);
    if (!isObj) {
        cout << "Failed to open file" << endl;
    }
    string c;
    int position = 0;
    while (isObj >> c) {
        if (position >= start && position <= end) {
            int current = stoi(c);
            nums.push_back(current);
        }
        position++;
    }
    isObj.close();
    return nums;
}

/**
 * Writes to the file
 * @param nums
 * @param len
 */
void ThreadSorting::writeToFile(vector<int> nums) {
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
    int* pair = new int[2];
    int num_1 = rand() % n;
    int num_2 = rand() % n;
    if (num_1 <= num_2) {
        pair[0] = num_1;
        pair[1] = num_2;
    }
    else {
        pair[0] = num_2;
        pair[1] = num_1;
    }
    return pair;
}

void ThreadSorting::swap(int* pointer1, int* pointer2) {
    int temp = *pointer1;
    *pointer1 = *pointer2;
    *pointer2 = temp;
}

int ThreadSorting::partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high- 1; j++) {
        if (arr[j] <= pivot) {
            i++;
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
    int a, b;
    for (a = 0; a < len-1; a++) {
        for (b = 0; b < len - a - 1; b++) {
            if (arr[b] > arr[b + 1]) {
                swap(&arr[b], &arr[b + 1]);
            }
        }
    }
    return arr;
}


/**
 *
 * @param r
 * @return
 */
void * ThreadSorting::computation(void *r) {
    pthread_mutex_lock(&lock2);

    // Generates a random pair of numbers ranging from 0 to n
    int* pair = generatePair(totalCount);
    int i = pair[0];
    int j = pair[1];

    //cout << "\nThread " << syscall(SYS_gettid)  << endl;
    cout << "\nPair (i, j) = (" << i << ", " << j << ")" << endl;
    int len = (j - i) + 1;
    int arr[len];

    // Read values from the file
    vector<int> nums;
    nums = readFromFile(nums, i, j);

    cout << "Subarray to be sorted: ";
    // iterate through the vector
    int idx = 0;
    for (int a = i; a <= j; a++) {
        arr[idx] = nums.at(idx);
        cout << arr[idx] << " ";
        idx++;
    }
    cout << "\nSubarray sorted with ";
    // Randomly select which sorting algorithm to use between
    // a) quick sort , b) insert sort, and c) bubble sort
    int sortingOption = rand() % 3;
    switch (sortingOption) {
        case 0: {
            cout << "quicksort: ";
            quicksort(arr, 0, len -1);
            break;
        }
        case 1: {
            cout << "insert sort: ";
            insertsort(arr, len);
            break;
        }
        case 2: {
            cout << "bubble sort: ";
            bubblesort(arr, len);
            break;
        }
    }

    nums.clear();
    vector<int> readValues;
    readValues = readFromFile(readValues, 0, totalCount -1);
    for (int idx = i, index = 0; idx <= j; ++idx) {
        readValues.at(idx) = arr[index];
        cout << arr[index] << " ";
        index++;
    }

    // Write sorted values to the file
    writeToFile(readValues);
    readValues.clear();
    cout << "\nArray after sorting in range (i, j): ";
    display(0, totalCount -1);
    // -----------------------------------------------------------------------------------
    // Thread manager
    thread_t manager;
    pthread_create(&manager, 0, isSorted, (void*) (long) 0);
    pthread_join(manager, NULL);

    pthread_mutex_unlock(&lock2);
    return (void*)0;
}

bool ThreadSorting::sortedList() {
    vector<int> wholeArray;
    wholeArray = readFromFile(wholeArray, 0, totalCount -1);
    if (wholeArray.size() == 0 || wholeArray.size() == 1) {
        return true;
    }
    for (int i = 1; i < wholeArray.size(); i++) {
        // Unsorted pair found
        if (wholeArray.at(i - 1) > wholeArray.at(i)) {
            return false;
        }
    }
    return true;
}
/**
 * Checks whether the list is completely sorted
 * @return
 */
void* ThreadSorting::isSorted(void * s) {
    pthread_mutex_lock(&lock1);
    bool isListSorted = sortedList();
    stop = isListSorted;
    // Check condition
    if (isListSorted) {
        cout << "\n------------------------------------->" << endl;
        cout << "Array at the end of the sorting process: " ;
        display(0, totalCount-1);
        cout << "Simulation completed " << endl;
        exit(2);
    }
    pthread_mutex_unlock(&lock1);
    return (void*) 0;
}
/**
 * Checks if a valid input was provided
 * @param input
 * @return
 */
bool ThreadSorting::inputCheck(string input) {
    // Check for wrong input type
    if (input.length() == 0 || input.length() > 7) {
        cout << "Please provide a positive integer." << endl;
        return false;
    }
    for (int idx = 0; idx < input.length(); ++idx) {
        if (!isdigit(input[idx])) {
            std::cout << "Please only provide a positive integer." << std::endl;
            return false;
        }
    }
    if (stoi(input) == 0) {
        cout << "The number of threads should be greater than 0."<< endl;
        return false;
    }
    return true;
}

/**
 * Main function for the program
 * @param n command line argument for the number of elements
 * @return
 */
int ThreadSorting::main(int n) {
    // Get the number of threads
    string threadsCount;
    cout << "Enter the number of threads: " << endl;
    getline(cin, threadsCount);
    bool input = inputCheck(threadsCount);

    while (!input) {
        cout << "Enter the number of threads: " << endl;
        getline(cin, threadsCount);
        input = inputCheck(threadsCount);
    }
    M = stoi(threadsCount);
    totalCount = n;
    vector<int> numbersGenerated;
    for (int o = 0; o < totalCount; o++) {
        int number = rand() % 1000 ;
        numbersGenerated.push_back(number);
    }
    writeToFile(numbersGenerated);
    cout << "\nArray before starting the sorting process: ";
    display(0, totalCount -1);
    cout << "------------------------------------->" << endl;


    if (pthread_mutex_init(&lock1, NULL) != 0) {
        printf("\n mutex init has failed for lock 1\n");
        return 1;
    }
    if (pthread_mutex_init(&lock2, NULL) != 0) {
        printf("\n mutex init has failed for lock 2\n");
        return 1;
    }

    // Start the M threads
    thread_t threads[M];

    while (!stop) {
        for (int i = 0; i < M; i++) {
            pthread_create(&threads[i], NULL, computation, (void *) (long) i);
        }
        // Wait for the threads to finish
        for (int i = 0; i < M; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    return 0;
}

void ThreadSorting::display(int start, int end) {
    vector<int> list;
    list = readFromFile(list, start, end);
    for (int i = start; i <= end; i++) {
        cout << list.at(i) << " ";
    }
    cout << endl;
}