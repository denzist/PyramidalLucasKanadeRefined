#include "image_processing.h"
#include<iostream>
#include <cmath>
#include <limits>
#include <string>
#include<assert.h>

namespace image_processing {

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// INTENSITY MATRIX
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

const Matrix<double> IntensityMatrix(const char* file_name)
{
    return IntensityMatrix(QImage(file_name));
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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Unknown Flow
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


Matrix<double> UnknownFlow()
{
    Matrix<double> f(1, 2);
    f.data(0) = std::numeric_limits<double>::max();
    f.data(1) = std::numeric_limits<double>::max();
    return f;
}

bool isUnkownFlow(const Matrix<double>& flow)
{
    return (flow.data(0) == std::numeric_limits<double>::max() && flow.data(1) == std::numeric_limits<double>::max());
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Iterative Lucas Kanade algorithm
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

//inverse 2x2 matrix, returns false if matrix not invertable
bool inverse(const Matrix<double>& g, Matrix<double>& g_inv)
{
    if(g.cols() != 2 || g.rows() != 2)
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"g are wrong sized\n";
        exit(-1);
    }
    double tau = 0.01;
    double det = g.data(0,0)*g.data(1,1) - g.data(0,1)*g.data(1,0);
    if(fabs(det) < tau)
    {
        return false;
    }
    //compute eigen values
    double tmp1 = (g.data(0,0) + g.data(1,1))/2.;
    double tmp2 = g.data(0,0) - g.data(1,1);
    double tmp3 = std::sqrt(4*g.data(1,0)*g.data(1,0) + tmp2*tmp2)/2.;
    double eigen_v1 = tmp1 - tmp3;
    //double eigen_v2 = tmp1 + tmp3;

    if(fabs(eigen_v1) < tau)
    {
        return false;
    }

    g_inv = Matrix<double>(2);
    g_inv.data(0,0) =  g.data(1,1) / det;
    g_inv.data(0,1) = -g.data(1,0) / det;
    g_inv.data(1,0) = -g.data(0,1) / det;
    g_inv.data(1,1) =  g.data(0,0) / det;
    return true;
}

//Refined Lucas Kanade flow at x and y point of the img_prev and the img_curr
//with precalculated der_x and der_y of the img_prev
const Matrix<double> LucasKanadeRefined(const int x, const int y,
                                         int window_size,
                                         const Matrix<double>& init_guess,
                                         const Matrix<double>& der_x, const Matrix<double>& der_y,
                                         const Matrix<double>& img_prev, const Matrix<double>& img_curr)
{
    if(img_curr.cols() != img_prev.cols() || img_prev.rows() != img_curr.rows() ||
       img_prev.cols() != der_x.cols() || img_curr.rows() != der_x.rows() ||
       img_curr.cols() != der_y.cols() || img_curr.rows() != der_y.rows())
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"Matrices are different sized\n";
        exit(-1);
    }
    if(y < 0 || y > img_curr.rows()-1 || x < 0 || x > img_curr.cols() - 1)
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"Wrong arguments x, y\n";
        exit(-1);
    }
    Matrix<double> gradient(2);
    int patch_y_start = y - window_size/2;
    int patch_y_end = y + window_size - window_size/2;
    int patch_x_start = x - window_size/2;
    int patch_x_end = x + window_size - window_size/2;
    for(int i =  patch_y_start; i < patch_y_end; ++i)
    {
        for(int j = patch_x_start; j < patch_x_end; ++j)
        {
            gradient.data(0,0) += der_x.getValue(j,i)*der_x.getValue(j,i);
            gradient.data(1,0) += der_x.getValue(j,i)*der_y.getValue(j,i);
            gradient.data(0,1) += der_x.getValue(j,i)*der_y.getValue(j,i);
            gradient.data(1,1) += der_y.getValue(j,i)*der_y.getValue(j,i);
        }
    }
    Matrix<double> gradient_inv(2, 2);
    if(!inverse(gradient, gradient_inv))
    {
        return Matrix<double>(1,2);
    }

    Matrix<double> flow = init_guess;
    for(int k = 0; k < 5; ++k)
    {
        Matrix<double> image_mismatch(1, 2);
        for(int i =  patch_y_start; i < patch_y_end; ++i)
        {
            double shifted_i = (double)i + flow.getValue(0,1);
            for(int j = patch_x_start; j < patch_x_end; ++j)
            {
                double shifted_j = (double)j + flow.getValue(0, 0);
                double t_delta = img_curr.getInterpolatedValue(shifted_j, shifted_i)
                        - img_prev.getValue(j,i);
                image_mismatch.data(0,0) += t_delta*der_x.getValue(j,i);
                image_mismatch.data(0,1) += t_delta*der_y.getValue(j,i);
            }
        }
        Matrix<double> flow_step(1,2);
        flow_step.data(0,0) = gradient_inv.getValue(0,0)*image_mismatch.getValue(0,0)
                + gradient_inv.data(1,0)*image_mismatch.getValue(0,1);
        flow_step.data(0,1) = gradient_inv.getValue(0,1)*image_mismatch.getValue(0,0)
                + gradient_inv.data(1,1)*image_mismatch.getValue(0,1);
        if(flow_step.norm() < 0.4)
            break;
        flow += flow_step;
    }
    if(!inRange(x + flow.data(0), y + flow.data(1), img_curr))
    {
        //std::cout<<"not in range\n";
        return Matrix<double>(1,2);
    }
    return flow;
}

//Refined Lucas Kanade calculates optical flow over all pixel at img_prev and img_curr
const Matrix<Matrix<double> > LucasKanadeRefined(int window_size,
                                                  const Matrix<Matrix<double> >& init_guess,
                                                  const Matrix<double>& img_prev, const Matrix<double>& img_curr)
{
    if(img_prev.cols() != img_curr.cols() || img_prev.rows() != img_curr.rows())
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"img_curr and img_prev are different sized\n";
        exit(-1);
    }
    Matrix<double> der_x = img_prev.convolute(DerivativeXKernel());
    Matrix<double> der_y = img_prev.convolute(DerivativeYKernel());
    Matrix<Matrix<double> > flows(img_curr.cols(), img_curr.rows());
    for(int x = 0; x < img_curr.cols(); ++x)
        for(int y = 0; y < img_curr.rows(); ++y)
            flows.data(x, y) = LucasKanadeRefined(x, y, window_size,
                                                  init_guess.data(x, y),
                                                  der_x, der_y,
                                                  img_prev, img_curr);
    return flows;
}

const Matrix<Matrix<double> > PyramidalLucasKanadeRefined(int window_size,  double max_error,
                                                          int smoothing_window_size, double sigma,
                                                          int max_scaling_level,
                                                          const Matrix<double>& img_prev,
                                                          const Matrix<double>& img_curr)
{
    if(img_prev.cols() != img_curr.cols() || img_prev.rows() != img_curr.rows())
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"Matrices are different sized\n";
        exit(-1);
    }
    Matrix<double> gaus = GaussianKernel(smoothing_window_size, sigma);
    Pyramid<double> scaling_pyramid_prev(img_prev.convolute(gaus), gaus, max_scaling_level);
    Pyramid<double> scaling_pyramid_curr(img_curr.convolute(gaus), gaus, max_scaling_level);

    Matrix<Matrix<double> > flows((scaling_pyramid_curr.getLayer(max_scaling_level - 1).cols() + 1)/2,
                                  (scaling_pyramid_curr.getLayer(max_scaling_level - 1).rows() + 1)/2);
    for(int x = 0; x < flows.cols(); ++x)
        for(int y = 0; y < flows.rows(); ++y)
            flows.data(x, y) = Matrix<double>(1, 2);
    for(int level = max_scaling_level - 1; level > -1; --level)
    {
        int cols = scaling_pyramid_curr.getLayer(level).cols();
        int rows = scaling_pyramid_curr.getLayer(level).rows();
        Matrix<Matrix<double> > init_guess(cols, rows);
        for(int x = 0; x < cols; ++x)
            for(int y = 0; y < rows; ++y)
                init_guess.data(x, y) = 2.*flows.data(x/2, y/2);
        flows = LucasKanadeRefined(window_size, init_guess,
                                   scaling_pyramid_prev.getLayer(level),
                                   scaling_pyramid_curr.getLayer(level));

        //QImage flow_img = getQImageFromFlow(flows);
        //QString s = "/home/den/images/Army/" +  QString::number(level) +".png";
        //flow_img.save(s);
    }
    handleFlowError(flows, max_error, img_prev, img_curr);
    return flows;
}

//marks the bad flows as UnkownFlow with big error in intensity and with big norm
void handleFlowError(Matrix<Matrix<double> >& flows, double max_error,
                     const Matrix<double>& img_prev, const Matrix<double>& img_curr)
{
    double norm_mean = 0.;
    Matrix<double> flow_norms(flows.cols(), flows.rows());
    for(int x = 0; x < flows.cols(); ++x)
        for(int y = 0; y < flows.rows(); ++y)
        {
            if(isUnkownFlow(flows.data(x,y)))
            {
                continue;
            }
            if(!inRange(x + flows.data(x,y).data(0), y + flows.data(x,y).data(1), img_curr))
            {
                //std::cout<<"not in range\n";
                flows.data(x, y) = UnknownFlow();
                continue;
            }
            double error = fabs(img_curr.getInterpolatedValue(x + flows.data(x,y).data(0), y + flows.data(x,y).data(1))
                                - img_prev.getValue(x, y));
            if(error > max_error)
            {
                //std::cout<<"error\n";
                flows.data(x,y) = UnknownFlow();
                continue;
            }
            flow_norms.data(x,y) = flows.data(x,y).norm();
            norm_mean += flow_norms.data(x,y);
        }
    norm_mean /= double(flows.total());
    double norm_s = 0.;
    for(int x = 0; x < flows.cols(); ++x)
        for(int y = 0; y < flows.rows(); ++y)
        {
            if(isUnkownFlow(flows.data(x,y)))
            {
                continue;
            }
            norm_s += (flow_norms.data(x,y) - norm_mean)*(flow_norms.data(x,y) - norm_mean);
        }
    norm_s = sqrt(1./double(flows.total() - 1) * norm_s);
    //5 sigma rule
    double left_norm = norm_mean - 5.*norm_s;
    double right_norm = norm_mean + 5.*norm_s;
    for(int x = 0; x < flows.cols(); ++x)
        for(int y = 0; y < flows.rows(); ++y)
        {
            if(isUnkownFlow(flows.data(x,y)))
            {
                continue;
            }
            if(flow_norms.data(x,y) < left_norm || flow_norms.data(x,y) > right_norm)
            {
                flows.data(x,y) = UnknownFlow();
            }
        }
    return;

}

}

