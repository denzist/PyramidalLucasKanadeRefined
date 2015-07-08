#ifndef MATRIX_H
#define MATRIX_H
#include<iostream>
#include <cmath>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Matrix and Convolution
// This class provides some basic operations and data members
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace image_processing {

template<class T>
class Matrix;

template<class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T> &m);

template<class T>
class Matrix
{
protected:
    T* data_;
    int rows_;
    int cols_;

public:
    Matrix();
    Matrix(int cols, int rows);
    Matrix(int size);
    Matrix(const T* data,int cols, int rows);
    Matrix(const T* data, int size);
    Matrix(const Matrix<T>& m);
    ~Matrix();

    Matrix<T>& operator =(const Matrix<T>& m);

    int rows() const{return rows_;}
    int cols() const{return cols_;}
    int total() const{return rows_*cols_;}

    T* data(){return data_;}
    const T* data() const{return data_;}
    T& data(int i){return data_[i];}
    const T& data(int i) const{return data_[i];}

    //returns the pointer to [i,j] eleemnt in the data
    T& data(int x, int y){return data_[y*cols_ + x];}
    const T& data(int x, int y) const{return data_[y*cols_ + x];}

    const T getValue(int x, int y) const;
    const T getInterpolatedValue(double x, double y) const;

    friend std::ostream& operator<< <>(std::ostream& os, const Matrix<T> &m);

    Matrix<T>& operator +=(const Matrix<T>& m);
    Matrix<T>& operator -=(const Matrix<T>& m);
    Matrix<T>& operator *=(double scalar);
    Matrix<T>& operator /=(double scalar);

    const Matrix<T> operator -() const;
    const Matrix<T> operator +(const Matrix<T>& m) const;
    const Matrix<T> operator -(const Matrix<T>& m) const;
    const Matrix<T> operator *(double scalar) const;
    const Matrix<T> operator /(double scalar) const;

    const T convolute(int p_x, int p_y, const Matrix<double>& ker) const;
    const Matrix<T> convolute(const Matrix<double>& ker) const;
    double norm() const;

};

template<class T>
bool inRange(T ob, T left, T right);

template<class T>
bool inRange(int x, int y);
}

#include"optical_flow/src/matrix.tpp"

#endif // MATRIX_H

