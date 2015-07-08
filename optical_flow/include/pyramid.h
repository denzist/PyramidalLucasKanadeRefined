#ifndef PYRAMID
#define PYRAMID

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Scaling Pyramid
// Pyramid stucture for image scaling
// scales using kernel defined by user (reduce method)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------


namespace image_processing
{

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

#include"optical_flow/src/pyramid.tpp"

#endif // PYRAMID

