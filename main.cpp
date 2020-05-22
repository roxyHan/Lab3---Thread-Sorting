#include <iostream>
#include "ThreadSorting.h"

int main(int argc, char* argv[]) {
    std::cout << "" << std::endl;
    srand(time(NULL));
    if (argc != 2) {
        std::cout << "The number 'n' needs to be provided!" << std::endl;
        return EXIT_FAILURE;
    }
    std::string number = argv[1];
    int n = std::stoi(number);
    ThreadSorting* sorting = new ThreadSorting();
    sorting->main(n);
    delete sorting;
    return 0;
}
