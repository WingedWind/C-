#include <iostream>
#include <cassert>

#include "matrix.hpp"

int main() {

    int m = 0;
    int n = 0;

    m = input_size();
    n = input_size();

    matrix::matrix <double> matrix{m, n};
    matrix::matrix <double> matrix1{m, n};
    matrix.input_matrix();
    matrix1.input_matrix();

    matrix::matrix <double> res_right = matrix;
    matrix::matrix <double> res_right1{m, n};
    res_right1 = matrix1;

    matrix::matrix <double> res_left = matrix + matrix1;
    matrix::matrix <double> res_left1{m, n};
    res_left1 = matrix - matrix1;

    std::cout << std::endl << std::endl;

    res_right.print_matrix();
    res_right1.print_matrix();
    res_left.print_matrix();
    res_left1.print_matrix();

    return 0;
}