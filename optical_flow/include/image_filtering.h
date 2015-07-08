#ifndef IMAGE_FILTERING_H
#define IMAGE_FILTERING_H
#include"matrix.h"
#include<iostream>
#include <cmath>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Filters
// Some image filters such as derivatives, gaussian and
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace image_processing
{

const Matrix<double> DerivativeXKernel();
const Matrix<double> DerivativeYKernel();
const Matrix<double> GaussianKernel(int window_size, double sigma);
}

#include"optical_flow/src/image_filtering.tpp"

#endif // IMAGE_FILTERING_H

