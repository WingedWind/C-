#include <iostream>
#include <cassert>

#include "matrix.hpp"

int main() {

    int m = 0;
    int n = 0;

    m = input_size();
    
    n = m;

    matrix::matrix <double> matrix{m, n};
    matrix.input_matrix();
    
    std::cout << matrix.gauss_jordan() << std::endl;

    return 0;
}