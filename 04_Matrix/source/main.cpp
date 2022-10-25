#include <iostream>
#include <cassert>

#include "matrix.hpp"

int main() {

    int m = 0;
    int n = 0;

    std::cin >> m;
    assert(std::cin.good());

    if (m == 0) {
        std::cout << "You have entered a matrix of zero size" << std::endl;
        return 0;
    }
    
    n = m;

    matrix::matrix <double> matrix{m, n};
    matrix.input_matrix();
    
    std::cout << round(matrix.gauss_jordan()) << std::endl;

    return 0;
}