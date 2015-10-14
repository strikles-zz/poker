#include "CPCA.h"

#include <iostream>

using namespace Eigen;

CPCA::CPCA(Eigen::MatrixXd& data) 
{
    int m = data.rows();
    int n = data.cols();

    // substract mean from each dimension
    for (int i = 0; i < m; ++i) 
	{
        double mean = data.row(i).mean();
        data.row(i).array() -= mean;
    }

    // calculate covariance matrix
    MatrixXd covariance = data.matrix() * data.transpose() * ((double) 1.0 / (n - 1));

    // calculate eigenvectors and eigenvalues
    SelfAdjointEigenSolver<MatrixXd> eigensolver(covariance);
    if (eigensolver.info() != Success) 
		abort();

    VectorXd v = eigensolver.eigenvalues();
    PC = eigensolver.eigenvectors();

    // sort variances in decresing order
    for (int i = v.rows(), j = 0; i > 1; --i, ++j)
	{
        int maxIndex;
        double maxCoeff = v.tail(i).maxCoeff(&maxIndex);
        v(j + maxIndex) = v(j);
        v(j) = maxCoeff;
        PC.col(j).swap(PC.col(j + maxIndex));
    }
}

CPCA::~CPCA()
{
}

void CPCA::performDimensionReduction(Eigen::MatrixXd& data, int dimensions) 
{
    ret = ((PC.transpose() * data).leftCols(dimensions));
}