#include"matrix.h"

namespace image_processing {

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MATRIX
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<class T>
Matrix<T>::Matrix()
{
    data_ = NULL;
    rows_ = 0;
    cols_ = 0;
}

//allocates memory in the data of size rows*cols
template<class T>
Matrix<T>::Matrix(int cols, int rows):
    rows_(rows), cols_(cols)
{
    if(rows < 0 || cols < 0)
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": " << "Negative values of rows and cols arguments" << '\n';
        std::exit(-1);
    }

    try
    {
        data_ = new T[total()];
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": " << ba.what() << '\n';
        std::exit(-1);

    }
    for(int i = 0; i < total(); ++i)
        data_[i] = T();
}

template<class T>
Matrix<T>::Matrix(int size):
    Matrix(size, size)
{

}

//this constructor makes deep copy of the data
//invalid arguments cause undefined behavior
template<class T>
Matrix<T>::Matrix(const T* data, int cols, int rows):
    Matrix(cols, rows)
{
    std::copy(data, data + total(), data_);
}

//this constructor makes deep copy of the data
//invalid arguments (size) cause undefined behavior
template<class T>
Matrix<T>::Matrix(const T* data, int size):
    Matrix(data, size, size)
{

}

//this constructor makes deep copy of the data
template<class T>
Matrix<T>::Matrix(const Matrix<T>& m):
    Matrix(m.data(), m.cols(), m.rows())
{
}
template<class T>
Matrix<T>::~Matrix()
{
    delete[] data_;
    data_ = NULL;
}

//performs deep copy of the data
template<class T>
Matrix<T>& Matrix<T>::operator =(const Matrix<T>& m)
{
    if(this == &m)
        return *this;

    delete[] data_;

    try
    {
        data_ = new T[m.total()];
    }
    catch (std::bad_alloc& ba)
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": " << ba.what() << '\n';
        std::exit(-1);

    }

    std::copy(m.data(), m.data() + m.total(), data_);
    rows_ = m.rows();
    cols_ = m.cols();
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator +=(const Matrix<T>& m)
{
    if(rows_ != m.rows() || cols_ != m.cols())
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"Matrices are different sized.\n";
        exit(-1);
    }

    for(int i = 0; i < m.rows(); ++i)
        for(int j = 0; j < m.cols(); ++j)
            data(j,i) += m.data(j,i);

    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator -=(const Matrix<T>& m)
{
    if(rows_ != m.rows() || cols_ != m.cols())
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"Matrices are different sized.\n";
        exit(-1);
    }

    for(int i = 0; i < m.rows(); ++i)
        for(int j = 0; j < m.cols(); ++j)
            data(j,i) -= m.data(j,i);

    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator *=(double scalar)
{
    for(int i = 0; i < total(); ++i)
        data_[i] = data_[i] * scalar;
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator /=(double scalar)
{
    for(int i = 0; i < total(); ++i)
        data_[i] = data_[i] / scalar;
    return *this;
}

template<class T>
const Matrix<T> Matrix<T>::operator +(const Matrix& m) const
{
    if(rows_ != m.rows() || cols_ != m.cols())
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"Matrices are different sized.\n";
        std::cerr<<m;
        exit(-1);
    }
    Matrix<T> result = *this;
    result += m;
    return result;
}

template<class T>
const Matrix<T> Matrix<T>::operator -(const Matrix<T>& m) const
{
    if(rows_ != m.rows() || cols_ != m.cols())
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"Matrices are different sized.\n";
        exit(-1);
    }
    Matrix<T> result = *this;
    result -= m;
    return result;
}

template<class T>
const Matrix<T> Matrix<T>::operator -() const
{
    Matrix<T> result(*this);
    for(int i = 0; i < total(); ++i)
            result.data(i) = - data_[i];
    return result;
}

template<class T>
const Matrix<T> Matrix<T>::operator *(double scalar) const
{
    Matrix result(*this);
    for(int i = 0; i < total(); ++i)
        result.data(i) = scalar * data_[i];
    return result;
}

template<class T>
const Matrix<T> operator * (double scalar, const Matrix<T>& m)
{
    return m*scalar;
}

template<class T>
const Matrix<T> Matrix<T>::operator /(double scalar) const
{
    Matrix result(*this);
    for(int i = 0; i < total(); ++i)
        result.data(i) = data_[i] / scalar;
    return result;
}


template<class T>
std::ostream& operator <<(std::ostream& os, const Matrix<T> &m)
{
    os<<"rows : "<< m.rows() << " cols : "<< m. cols()<<"\n";
    for(int i = 0; i < m.rows(); ++i)
    {
        for(int j = 0; j < m.cols(); ++j)
            os<<m.data(j,i)<<" ";
        os<<"\n";
    }
    return os;
}

template<class T>
const T Matrix<T>::getValue(int x, int y) const
{
    if(x < 0 || x >= cols() || y < 0 || y >= rows())
        return T();
    return data(x, y);
}

//bilinear interpolation for subpixel values
template<class T>
const T Matrix<T>::getInterpolatedValue(double x, double y) const
{
    int floor_x = (int)x;
    int floor_y = (int)y;
    int ceiling_x = floor_x + 1;
    int ceiling_y = floor_y+ 1;
    double a_x = double(ceiling_x) - x;
    double a_y = double(ceiling_y) - y;
    double b_x = x - double(floor_x);
    double b_y = y - double(floor_y);
    return a_x*a_y*getValue(floor_x, floor_y) + b_x*a_y*getValue(ceiling_x, floor_y)
            + a_x * b_y*getValue(floor_x, ceiling_y) + b_x*b_y*getValue(ceiling_x, ceiling_y);
}

template<>
double Matrix<double>::norm() const
{
    double result = 0.;
    for(int i = 0; i < total(); ++i)
        result += data(i) * data(i);
    return std::sqrt(result);
}

template<>
double Matrix<int>::norm() const
{
    int result = 0.;
    for(int i = 0; i < total(); ++i)
        result += data(i) * data(i);
    return std::sqrt((double)result);
}

template<class T>
bool inRange(T ob, T left, T right)
{
    return (ob >= left && ob < right);
}

template<class T>
bool inRange(int x, int y, const Matrix<T>& m)
{
    return (inRange(x, 0, m.cols() - 1) && inRange(y, 0, m.rows() - 1));
}

template<class T>
bool inRange(double x, double y, const Matrix<T>& m)
{
    return (inRange(x, 0., double(m.cols() - 1 )) && inRange(y, 0., double(m.rows() - 1)));
}

}
