#pragma once

#include <iostream>
#include <cassert>
#include <cmath>

namespace matrix {

    template <typename Type> struct matrix final {

        private:
            
            int m_;
            int n_;
            Type **matrix_;
            
            struct  ProxyRow {
                Type *row;
                
                Type &operator[](int n) {
                    return row[n];
                }
                const Type &operator[](int n) const {
                    return row[n];
                }
            };

            const double EPSILON = 10E-3;
            
        
        public:
            
            matrix(int m, int n): m_(m), n_(n) {
                create_matrix();
            }

//-----------------------------------------rule of five----------------------------------------//

            matrix(const matrix &rhs): m_(rhs.m_), n_(rhs.n_) {
                
                create_matrix();
                copy_matrix(rhs);
            }

            matrix( matrix &&rhs) noexcept: m_(rhs.m_), n_(rhs.n_), matrix_(rhs.matrix_) {
                rhs.matrix_ = nullptr;
            }


            matrix operator= (const matrix &rhs) {
                
                if (this == &rhs) {
                    return *this;
                }
                
                for (int i = 0; i < m_; ++i) {
                    delete [] matrix_[i];
                }
                delete [] matrix_;
                
                n_ = rhs.n_;
                m_ = rhs.m_;

                create_matrix();
                copy_matrix(rhs);

                return *this;
            }

            matrix operator= (matrix &&rhs) noexcept {
                if (this == &rhs) {
                    return *this;
                }

                std::swap(m_, rhs.m_);
                std::swap(n_, rhs.n_);
                std::swap(matrix_, rhs.matrix_);

                return *this;
            }

            ~matrix() {

                for (int i = 0; i < m_; ++i)
                    delete [] matrix_[i];

                delete [] matrix_;
            }

//---------------------------------------------------------------------------------------------//

            ProxyRow operator[] (int m) const {
                
                ProxyRow tmp {matrix_[m]};
                return tmp;
            }

            matrix operator- (const matrix<Type> &tmp) {
                
                if (tmp.m_ != m_ || tmp.n_ != n_) {
                    assert(tmp.m_ != m_ || tmp.n_ != n_);
                }

                matrix result{m_, n_};

                for (int i = 0; i < m_; ++i) {
                    for (int j = 0; j < n_; ++j) {
                        result.matrix_[i][j] = matrix_[i][j] - tmp.matrix_[i][j];
                    }
                }

                return result;
            }

            matrix operator+ (const matrix<Type> &tmp) {
                
                if (tmp.m_ != m_ || tmp.n_ != n_) {
                    assert(tmp.m_ != m_ || tmp.n_ != n_);
                }

                matrix result{m_, n_};

                for (int i = 0; i < m_; ++i) {
                    for (int j = 0; j < n_; ++j) {
                        result.matrix_[i][j] = matrix_[i][j] + tmp.matrix_[i][j];
                    }
                }

                return result;
            }

            matrix operator- () {

                matrix result{m_, n_};

                for (int i = 0; i < m_; ++i) {
                    for (int j = 0; j < n_; ++j) {
                        result.matrix_[i][j] = matrix_[i][j] * -1;
                    }
                }
                return result;
            }

            void create_matrix() {
                
                matrix_ = new Type*[m_]; 
                for (int i = 0; i < m_; ++i) {
                    matrix_[i] = new Type[n_] {};
                }
            }

            void copy_matrix(const matrix<Type> &matrix) {

                if (matrix.m_ != m_ || matrix.n_ != n_) {
                    assert(matrix.m_ != m_ || matrix.n_ != n_);
                }

                for (int i = 0; i < m_; ++i) {
                    std::copy(matrix.matrix_[i], matrix.matrix_[i] + n_, matrix_[i]);
                }
            }

            void input_matrix() {
                for (int i = 0; i < m_; ++i) {
                    for (int j = 0; j < n_; ++j) {
                        std::cin >> matrix_[i][j];
                        assert(std::cin.good());
                    }
                }
            }

//------------------------------------------algorithm------------------------------------------//

            void zero_column (int &start) {
                Type num = matrix_[start][start];
                double coef = 1;

                for (int i = start + 1; i < m_; ++i) {
                    coef = matrix_[i][start] / num;
                    
                    for (int j = start; j < m_; ++j) {
                        matrix_[i][j] -= coef * matrix_[start][j];
                    }
                    
                }
                ++ start;
            }

            Type max_element(int start, int &line, int &column) {
                
                Type max = matrix_[start][start];

                for (int i = start; i < m_; ++i) {
                    for (int j = start; j < m_; ++j) {
                        if (std::abs(matrix_[i][j]) > max) {
                            max = std::abs(matrix_[i][j]);
                            line = i;
                            column = j;
                        }
                    }
                }
                
                return max;
            }

            int pivot (int start, int &flag) {
                
                int line = start, column = start;
                Type max = max_element(start, line, column), change = 0;

                if (max == 0) {
                    return -1;
                }

                if (column != start) {
                    
                    flag *= -1;
                    
                    for (int i = 0; i < m_; ++i) {
                        change = matrix_[i][start];
                        matrix_[i][start] = matrix_[i][column];
                        matrix_[i][column] = change;
                    }
                }

                if (line != start) {
                    
                    flag *= -1;
                    
                    for (int j = 0; j < m_; ++j) {
                        change = matrix_[start][j];
                        matrix_[start][j] = matrix_[line][j];
                        matrix_[line][j] = change;
                    }
                }

                return 0;
            }

            Type determinant (int &flag) {
                
                Type res = 1;
                
                for (int i = 0; i < m_; ++i) {
                    res *= matrix_[i][i];
                }

                res *= flag;
                if (res >= 0) {
                    return res + EPSILON;
                }
                return res - EPSILON;
            }

            Type gauss_jordan() {
                
                int start = 0, flag = 1;

                for (int i = 0; i < m_; ++i) {
                    if(pivot(start, flag) == -1) {
                        return 0;
                    }
                    zero_column(start);
                }
                
                return round(determinant(flag));
            }

//---------------------------------------------------------------------------------------------//

//---------------------------------------debug functions---------------------------------------//

            void print_matrix() {

                for (int i = 0; i < m_; ++i) {
                    for (int j = 0; j < n_; ++j) {
                        std::cout << matrix_[i][j] << " ";
                    }

                    std::cout << std::endl;
                }

                std::cout << std::endl;
            }
//---------------------------------------------------------------------------------------------//
    };
}

void imput_size(int &size) {

    std::cin >> size;
    assert(std::cin.good());

    if (size == 0) {
        std::cout << "You have entered a matrix of zero size" << std::endl;
        assert(size == 0);
    }
}