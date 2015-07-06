#ifndef IMAGE_FILTERING_H
#define IMAGE_FILTERING_H
#include"matrix.h"
#include<iostream>
#include <cmath>

namespace image_processing
{

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FILTERS
// Filters for image filtering
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<class T>
const Matrix<T> initMatrix(const T& value, int width, int hight);
const Matrix<double> DerivativeXKernel();
const Matrix<double> DerivativeYKernel();
const Matrix<double> GaussianKernel(int window_size, double sigma);


double pixelDistance(int x1, int y1, int x2, int y2);

//if sigma ~ 0 undefined values
const Matrix<double> getDistanceWeightFilter(int window_size, double sigma);

template<class T, typename... Args>
const Matrix<double> getWeightFilter(int x, int y, int window_size,
                               const T& elem, const Matrix<T>& J,
                               double (*weight_fun)(const T&, const T&, Args...), Args... args);

//if sigma ~ 0 undefined values
template<class T>
double colorWeight(const T& a, const T& b, double sigma);

}

#include"image_filtering.tpp"

#endif // IMAGE_FILTERING_H

