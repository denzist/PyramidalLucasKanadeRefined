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
    Matrix<Matrix<double> > flows = PyramidalLucasKanadeRefined(15, 5., 5, 0.6, 4,  J, I);
    QImage flow_img = getQImageFromFlow(flows);
    flow_img.save("/home/den/images/Army/flow6.png");
    return 0;
    //return app.exec();
}
