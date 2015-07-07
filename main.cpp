//#include <QApplication>
//#include <QQmlApplicationEngine>
#include "image_processing.h"
#include<iostream>
#include<iomanip>

using namespace image_processing;

int main(int argc, char *argv[])
{

    //std::cout << std::fixed;
    //std::cout << std::setprecision(2);
    Matrix<double> I = IntensityMatrix("/home/den/images/Army/frame10.png");

    Matrix<double> J = IntensityMatrix("/home/den/images/Army/frame11.png");
    Matrix<Matrix<double> > flows = PyramidalLucasKanadeRefined(15, 5., 5, 0.6, 4,  I, J);
    QImage flow_img = getQImageFromFlows(flows);
    flow_img.save("/home/den/images/Army/flow7.png");
    /*double* a = new double[10];
    for(int i = 0; i < 10; ++i)
        a[i] = 10 -i;
    for(int i = 0; i < 10; ++i)
        std::cout<<a[i]<<" ";
    std::cout<<"\n";
    std::sort(a, a + 10);
    for(int i = 0; i < 10; ++i)
        std::cout<<a[i]<<" ";
    std::cout<<"\n";
*/

    return 0;
    //return app.exec();
}
