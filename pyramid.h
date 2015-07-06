#ifndef PYRAMID
#define PYRAMID

namespace image_processing
{

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SCALING PYRAMID
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<class T>
class Pyramid
{
    std::vector<Matrix<T> > layers_;
    Matrix<double> ker_;
public:
    Pyramid(const Matrix<T>& m, const Matrix<double>& ker, int layers);
    int size() const{return layers_.size();}
    const Matrix<T>& getLayer(int layer) const;
    Matrix<T>& getLayer(int layer);
    void reduce();
};

}

#include"pyramid.tpp"

#endif // PYRAMID

