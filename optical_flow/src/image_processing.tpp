#include "optical_flow/include/image_processing.h"
#include"optical_flow/include/lucas_kanade.h"
#include<iostream>
#include <cmath>
#include <limits>
#include <string>
#include<assert.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// INTENSITY MATRIX
// The matrix of the image intensity
// Plus some operatoins to visualization of optical flow with qt images
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace image_processing {

const Matrix<double> IntensityMatrix(const QImage& qimage)
{
    Matrix<double> m(qimage.width(), qimage.height());
    for(int i = 0; i < m.rows(); ++i)
        for(int j = 0; j < m.cols(); ++j)
        {
            m.data(j, i) = double(qGray(qimage.pixel(j, i)));
        }
    return m;

}

const Matrix<double> IntensityMatrix(const std::string& file_name)
{
    return IntensityMatrix(QImage(file_name.c_str()) );
}

const QImage getQImage(Matrix<double>& m)
{
    //std::cout<<"width() = "<< cols()<<" height = "<<rows()<<"\n";
    QImage img(m.cols(), m.rows(), QImage::Format_RGB32);
    double min = m.data(0);
    double max = m.data(0);
    for(int i = 1; i < m.total(); ++i)
    {
        if(m.data(i) > max)
            max = m.data(i);
        if(m.data(i) < min)
            min = m.data(i);
    }
    for(int i = 0; i < m.rows(); ++i)
        for(int j = 0; j < m.cols(); ++j)
        {
            //int color = std::max(0, std::min(255, (int)data(i, j)));
            int color = (int) max;
            if(max - min != 0)
            {
                color = (int)((m.data(j, i) - min)*255./(max - min));
            }
            img.setPixel(j, i, qRgb(color, color, color));
        }

    return img;
}

//calulates hsv value and then turn it into QRgb
const QRgb getQRgbFromFlow(const Matrix<double>& flow, double max_flow_norm)
{
    double flow_x = flow.data(0);
    double flow_y = flow.data(1);
    if(isUnkownFlow(flow))
         return qRgb(0, 0, 0);
    if(flow.norm() < 0.1)
         return qRgb(255, 255, 255);
     //HSV model
    double flow_norm = flow.norm();
    double flow_angle = 0.;
    if(flow_norm != 0.)
        flow_angle = asin(flow_y/flow_norm);
    if(flow_x < 0)
        flow_angle = M_PI - flow_angle;
    if(flow_angle < 0.)
        flow_angle += 2*M_PI;
    double seturation = flow_norm / max_flow_norm;
    double hue = flow_angle * 180./M_PI;
    double v_min = 1. - seturation;
    double a = (1 - v_min)*double((int)hue % 60)/60.;
    double v_inc = v_min + a;
    double v_dec = 1 - a;
    int hue_i = int(hue / 60.)%6;
    switch(hue_i)
    {
    case 0:
        return qRgb(255, int(v_inc*255.), int(v_min*255));
    case 1:
        return qRgb(int(v_dec*255.), 255, int(v_min*255));
    case 2:
        return qRgb(int(v_min*255.), 255, int(v_inc*255));
    case 3:
        return qRgb(int(v_min*255.), int(v_dec*255), 255);
    case 4:
        return qRgb(int(v_inc*255.), int(v_min*255), 255);
    case 5:
        return qRgb(255, int(v_min*255), int(v_dec*255.));
    }
}

//turns Optical Flow Matrix into QImage
const QImage getQImageFromFlows(const Matrix<Matrix<double> >& flows)
{
    QImage img(flows.cols(), flows.rows(), QImage::Format_RGB32);
        double max_flow_norm = 0.;
        for(int y = 0; y < flows.rows(); ++y)
            for(int x = 0; x < flows.cols(); ++x)
            if(flows.data(x, y).norm() > max_flow_norm && !isUnkownFlow(flows.data(x, y)))
            {
                //std::cout<<x<<" "<<y<<"\n"<<flow.data(x, y);
                max_flow_norm = flows.data(x, y).norm();
                //std::cout<<max_flow_norm<<"\n";
            }
        //max_flow_norm = 7.;
        for(int y = 0; y < flows.rows(); ++y)
            for(int x = 0; x < flows.cols(); ++x)
            {
                img.setPixel(x, y, getQRgbFromFlow(flows.data(x, y), max_flow_norm));
            }

        return img;
}

}

