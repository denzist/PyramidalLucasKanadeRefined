#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include<QImage>
#include <algorithm>
#include<memory>

#include"matrix.h"
#include"pyramid.h"
#include"image_filtering.h"

namespace image_processing {

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// INTENSITY MATRIX
// The matrix of the image intensity
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

const Matrix<double> IntensityMatrix(const QImage& qimage);

const Matrix<double> IntensityMatrix(const char* file_name);

const QImage getQImage(Matrix<double>& m);

const QRgb getqRgbFromFlow(const Matrix<double>& flow, double max_flow_norm);

const QImage getQImageFromFlow(Matrix<Matrix<double> > flow);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Unkown flow
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

Matrix<double> UnknownFlow();

bool isUnkownFlow(const Matrix<double>& flow);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Iterative Lucas Kanade algorithm
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

//inverse 2x2 matrix, returns false if matrix not invertable
bool inverse(const Matrix<double>& g, Matrix<double>& g_inv);

//Refined Lucas Kanade flow at x and y point of the img_prev and the img_curr
//with precalculated der_x and der_y of the img_curr
const Matrix<double> LucasKanadeRefined(const int x, const int y,
                                         int window_size,
                                         const Matrix<double>& init_guess,
                                         const Matrix<double>& der_x, const Matrix<double>& der_y,
                                         const Matrix<double>& img_prev, const Matrix<double>& img_curr);


const Matrix<Matrix<double> > LucasKanadeRefined(int window_size,
                                                  const Matrix<Matrix<double> >& init_guess,
                                                  const Matrix<double>& img_prev, const Matrix<double>& img_curr);

const Matrix<Matrix<double> > PyramidalLucasKanadeRefined(int window_size,  double max_error,
                                                          int smoothing_window_size, double sigma,
                                                          int max_scaling_level,
                                                          const Matrix<double>& img_prev,
                                                          const Matrix<double>& img_curr);

}

#include<image_processing.tpp>

#endif // IMAGE_PROCESSING_H
