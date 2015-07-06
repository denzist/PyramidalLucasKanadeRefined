#include"image_filtering.h"

namespace image_processing
{

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// CONVOLUTION
// We assume that kernel is a Matrix with center in the (rows()/2, cols()/2)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


template<>
const double Matrix<double>::convolute(int x, int y, const Matrix<double>& ker) const
{
    double result = 0.;
    for(int i =  0; i < ker.rows(); ++i)
    {
        int m_i = y + ker.rows()/2 - i;
        for(int j = 0; j < ker.cols(); ++j)
        {
            int m_j = x + ker.cols()/2 - j;
            result += ker.getValue(j, i) * getValue(m_j, m_i);
        }
    }
    return result;
}

template<class T>
const Matrix<T> Matrix<T>::convolute(const Matrix<double>& ker) const
{
    Matrix<T> result = *this;
    for(int i = 0; i < result.rows(); ++i)
        for(int j = 0; j < result.cols(); ++j)
            result.data(j, i) = convolute(j, i, ker);
    return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FILTERS
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

const Matrix<double> DerivativeXKernel()
{
    double data[] = {-1./2, 0., 1./2};
    return Matrix<double>(data, 3, 1);
}

const Matrix<double> DerivativeYKernel()
{
    double data[] = {-1./2, 0., 1./2};
    return Matrix<double>(data, 1, 3);
}

const Matrix<double> GaussianKernel(int window_size, double sigma)
{
    Matrix<double> ker(window_size);
    for(int i = 0; i < window_size; ++i)
        for(int j = 0; j < window_size; ++j)
            ker.data(i, j) = 1/ (2*M_PI * sigma * sigma)
                    *exp(-((i - window_size/2)*(i - window_size/2)
                    + (j - window_size/2)*(j - window_size/2))/(2*sigma*sigma));
    return ker;
}

const Matrix<double> BoxKernel(int window_size)
{
    Matrix<double> ker(window_size);
    for(int i = 0; i < window_size; ++i)
        for(int j = 0; j < window_size; ++j)
            ker.data(i, j) = 1./double(window_size*window_size);
    return ker;
}

const Matrix<double> DerivativeXGaussianKernel(int window_size, double sigma)
{
    Matrix<double> gaus_ker = GaussianKernel(window_size, sigma);
    return gaus_ker.convolute(DerivativeXKernel());
}

const Matrix<double> DerivativeYGaussianKernel(int window_size, double sigma)
{
    Matrix<double> gaus_ker = GaussianKernel(window_size, sigma);
    return gaus_ker.convolute(DerivativeYKernel());
}


}
