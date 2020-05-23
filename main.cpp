#include <iostream>
#include <climits>
#include "ThreadSorting.h"

// helper function
bool inputCheck(std::string input) {
    // Check for wrong input type
    if (input.length() == 0 || input.length() > 7) {
        std::cout << "Please provide a positive integer." << std::endl;
        return false;
    }
    for (int idx = 0; idx < input.length(); ++idx) {
        if (!isdigit(input[idx])) {
            std::cout << "Please only provide a positive integer." << std::endl;
            return false;
        }
    }
    if (stoi(input) == 0) {
        std::cout << "The number of threads should be greater than 0."<< std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    if (argc != 2) {
        std::cout << "Only the number of elements to generate needs to be provided!" << std::endl;
        return EXIT_FAILURE;
    }
    std::string number = argv[1];
    if (!inputCheck(number)) { return EXIT_FAILURE; }
    int n = std::stoi(number);
    ThreadSorting* sorting = new ThreadSorting();
    sorting->main(n);
    delete sorting;
    return 0;
}

