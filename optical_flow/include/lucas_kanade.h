#ifndef LUCAS_KANADE_H
#define LUCAS_KANADE_H

namespace image_processing {

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Unkown flow
// Special matrix with very big values
// Shows that the flow in the point is unknown
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

//Refined Lucas Kanade calculates optical flow over all pixel at img_prev and img_curr
const Matrix<Matrix<double> > LucasKanadeRefined(int window_size,
                                                  const Matrix<Matrix<double> >& init_guess,
                                                  const Matrix<double>& img_prev, const Matrix<double>& img_curr);


//with pyramids get init guesses and calculates optical flow
//in the end mark as UnkownFlow if its has a big value of the norm and
//error in intensity is bigger than max_error
const Matrix<Matrix<double> > PyramidalLucasKanadeRefined(int window_size,  double max_error,
                                                          int smoothing_window_size, double smoothing_sigma,
                                                          int max_scaling_level,
                                                          const Matrix<double>& img_prev,
                                                          const Matrix<double>& img_curr);

//marks the bad flows as UnkownFlow if it has a big error in intensity
//or its norm don't lay in (-5*norm_sigma, 5*norm_sigma) interval
void handleFlowError(Matrix<Matrix<double> >& flows, double max_error,
                     const Matrix<double>& img_prev, const Matrix<double>& img_curr);

}

#include"optical_flow/src/lucas_kanade.tpp"

#endif // LUCAS_KANADE_H

