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
            
            struct ProxyRow {
                Type *row;
                
                Type &operator[](int n) {
                    return row[n];
                }
                const Type &operator[](int n) const {
                    return row[n];
                }
            };

            const double EPSILON = 10E-3;
            
            void create_matrix() {
                
                matrix_ = new Type*[m_]; 
                assert (matrix_ != nullptr);
                for (int i = 0; i < m_; ++i) {
                    matrix_[i] = new Type[n_] {};
                    assert (matrix_[i] != nullptr);
                }
            }
        
        public:
            
            matrix(int m, int n): m_(m), n_(n) {

                assert(m > 0 && n > 0);
                create_matrix();
            }

//-----------------------------------------rule of five----------------------------------------//

            matrix(const matrix &rhs): m_(rhs.m_), n_(rhs.n_) {
                create_matrix();
                copy_matrix(rhs);
            }

            matrix( matrix &&rhs) noexcept: m_(rhs.m_), n_(rhs.n_), matrix_(rhs.matrix_) {
                
                rhs.matrix_ = nullptr;
                rhs.m_ = 0;
                rhs.n_ = 0;
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

//--------------------------------overload five for another type-------------------------------//    
            template <typename U> matrix(const matrix<U> &rhs): m_(rhs.height()), n_(rhs.width()) {
                
                create_matrix();
                copy_matrix(rhs);
            }

            template <typename U> matrix operator= (const matrix<U> &rhs) {
                
                if (this == &rhs) {
                    return *this;
                }
                
                for (int i = 0; i < m_; ++i) {
                    delete [] matrix_[i];
                }
                delete [] matrix_;
                
                n_ = rhs.width();
                m_ = rhs.hight();

                create_matrix();
                copy_matrix(rhs);

                return *this;
            }


//---------------------------------------------------------------------------------------------//

            ProxyRow operator[] (int m) const {
                
                ProxyRow tmp {matrix_[m]};
                return tmp;
            }

            matrix &operator+= (const matrix<Type> &tmp) {

                assert(tmp.m_ == m_ && tmp.n_ == n_);

                for (int i = 0; i < m_; ++i) {
                    for (int j = 0; j < n_; ++j) {
                        matrix_[i][j] = matrix_[i][j] + tmp.matrix_[i][j];
                    }
                }

                return *this;
            }

            matrix &operator-= (const matrix<Type> &tmp) {

                assert(tmp.m_ == m_ && tmp.n_ == n_);
                
                for (int i = 0; i < m_; ++i) {
                    for (int j = 0; j < n_; ++j) {
                        matrix_[i][j] = matrix_[i][j] - tmp.matrix_[i][j];
                    }
                }

                return *this;
            }

            matrix operator- () const {

                matrix result{m_, n_};

                for (int i = 0; i < m_; ++i) {
                    for (int j = 0; j < n_; ++j) {
                        result.matrix_[i][j] = matrix_[i][j] * -1;
                    }
                }
                return result;
            }

            Type *return_it_elem(int n) const{
                return matrix_[n];
            }

            template <typename U> void copy_matrix(const matrix<U> &matrix) const {

                assert(matrix.width() == m_ && matrix.height() == n_);

                for (int i = 0; i < m_; ++i) {
                    std::copy(matrix.return_it_elem(i), matrix.return_it_elem(i) + n_, matrix_[i]);
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

            int height() const{
                return m_;
            }

            int width() const{
                return n_;
            }

//------------------------------------------algorithm------------------------------------------//
        private:

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
                        std::swap(matrix_[i][start], matrix_[i][column]);
                    }
                }

                if (line != start) {
                    
                    flag *= -1;
                    
                    for (int j = 0; j < m_; ++j) {
                        std::swap(matrix_[start][j], matrix_[line][j]);
                    }
                }

                return 0;
            }

            double determinant (int &flag) {
                
                double res = 1;
                
                for (int i = 0; i < m_; ++i) {
                    res *= matrix_[i][i];
                }

                res *= flag;
                if (res >= 0) {
                    return res + EPSILON;
                }
                return res - EPSILON;
            }

        public:

            double gauss_jordan_for_double() {
                
                assert(m_ == n_);

                int start = 0, flag = 1;

                for (int i = 0; i < m_; ++i) {
                    if(pivot(start, flag) == -1) {
                        return 0;
                    }
                    zero_column(start);
                }
                
                return round(determinant(flag));
            }

            double gauss_jordan() {

                if (std::is_same_v<Type, double>) {
                    return gauss_jordan_for_double();
                }

                matrix <double> matrix_double = *this;

                return matrix_double.gauss_jordan_for_double();
            }

//---------------------------------------------------------------------------------------------//
//---------------------------------------debug functions---------------------------------------//

            void print_matrix() const{

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

    template <typename Type> matrix<Type> operator- (const matrix<Type> &lhs, const matrix<Type> &rhs){
    
        assert(lhs.width() == rhs.width() && lhs.height() == rhs.height());

        matrix result = lhs;
        result -= rhs;

        return result;
    }

    template <typename Type> matrix<Type> operator+ (const matrix<Type> &lhs, const matrix<Type> &rhs) {
        
        assert(lhs.width() == rhs.width() && lhs.height() == rhs.height());

        matrix result = lhs;
        result += rhs;

        return result;
    }
}

inline int input_size() {

    int size = 0;
    std::cin >> size;
    assert(std::cin.good());

    if (size == 0) {
        std::cout << "You have entered a matrix of zero size" << std::endl;
        assert(size != 0);
    }

    return size;
}