#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include<QImage>
#include<algorithm>
#include<memory>

#include"matrix.h"
#include"pyramid.h"
#include"image_filtering.h"
#include<string>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Intensity matrix
// The matrix of the image intensity
// Plus some operatoins to visualization of optical flow with qt images
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace image_processing {


const Matrix<double> IntensityMatrix(const QImage& qimage);

const Matrix<double> IntensityMatrix(const std::string& file_name);

const QImage getQImage(Matrix<double>& m);

//calulates hsv value and then turn it into QRgb
const QRgb getQRgbFromFlow(const Matrix<double>& flow, double max_flow_norm);

//turns Optical Flow Matrix into QImage
const QImage getQImageFromFlows(const Matrix<Matrix<double> >& flows);

}

#include"optical_flow/src/image_processing.tpp"

#endif // IMAGE_PROCESSING_H
