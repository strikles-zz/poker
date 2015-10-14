#ifndef LINEAR_REGR
#define LINEAR_REGR 1

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "dataset.h"
using namespace boost::numeric::ublas;

// sigmoid function

inline double sigmoid(double x) { return 1.0 / (1 + exp(-x)); }

//
// linerRegression with Gradient Descendant
//
//  alpha is the learning parame
//
//  numIterations is the num of intertion
//
//  Train is the training datset
//
//  the learnt vector a0, a1, a2 is returned

vector<double> linearRegressionDescGrad(double alpha, unsigned int numIterations,
										Dataset::Train & t);

//
// linerRegression with Stocastic Gradient Descendant
//
//  alpha is the learning parame
//
//  numIterations is the num of intertion
//
//  Train is the training datset
//
//  the learnt vector a0, a1, a2 is returned

vector<double> linearRegressionStocDescGrad(double alpha, unsigned int numIterations,
											Dataset::Train & t);

#endif
