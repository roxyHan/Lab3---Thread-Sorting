//
// Created by hanifa on 5/14/20.
//

#ifndef LAB3_THREADSORTING_H
#define LAB3_THREADSORTING_H

#include <string>
#include <vector>

class ThreadSorting {

public:
    ThreadSorting();
    static void infinite();
    static bool sortedList();
    static void* isSorted(void * m);
    static int* generatePair(int x);

    static void swap(int* p1, int* p2);
    static int partition(int arr[], int low, int high);

    static void quicksort(int arr[], int low, int high);
    static int* insertsort(int* arr, int x);
    static int* bubblesort(int* arr, int x);

    static std::vector<int> readFromFile(std::vector<int> arr, int start , int end);
    static void writeToFile(std::vector<int> xchar);


    static void* computation(void* r);
    int main(int n);
    static void display(int start, int end);
};


#endif //LAB3_THREADSORTING_H
