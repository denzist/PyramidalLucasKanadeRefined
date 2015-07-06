#include"pyramid.h"

namespace image_processing
{

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// SCALING PYRAMID
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<class T>
Pyramid<T>::Pyramid(const Matrix<T>& m, const Matrix<double>& ker, int layers)
{
    if(layers < 1)
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"The number of layers should be more than 1\n";
        exit(-1);
    }
    layers_.push_back(m);
    ker_ = ker;

    for(int i = 1; i < layers; ++i)
    {
        reduce();
    }
}

template<class T>
void Pyramid<T>::reduce()
{
    int last_layer = layers_.size() - 1;
    const Matrix<T>& prev_m = layers_[last_layer];
    int curr_cols = (prev_m.cols() + 1)/2;
    int curr_rows = (prev_m.rows() + 1)/2;
    Matrix<T> curr_m(curr_cols, curr_rows);
    for(int i = 0; i < curr_rows; ++i)
        for(int j = 0; j < curr_cols; ++j)
            curr_m.data(j, i) = prev_m.convolute(2*j, 2*i, ker_);
    layers_.push_back(curr_m);
}

template<class T>
const Matrix<T>& Pyramid<T>::getLayer(int layer) const
{
    if(layer < 0 || layer > layers_.size())
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"Bad argument. int layer is less than 0 or bigger than size()\n";
        exit(-1);
    }
    return layers_[layer];
}

template<class T>
Matrix<T>& Pyramid<T>::getLayer(int layer)
{
    if(layer < 0 || layer > layers_.size())
    {
        std::cerr<< "In "<< __PRETTY_FUNCTION__<<": ";
        std::cerr<<"Bad argument. int layer is less than 0 or bigger than size()\n";
        exit(-1);
    }
    return layers_[layer];
}

}
