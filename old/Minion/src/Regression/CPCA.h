#pragma once

#include <Eigen/Dense>

class CPCA
{
public:
    CPCA(Eigen::MatrixXd& data);
    virtual ~CPCA();

    void performDimensionReduction(Eigen::MatrixXd& data, int dimensions);
    Eigen::MatrixXd* getPCMatrix() { return &PC; }

private:
    Eigen::MatrixXd PC;
	Eigen::MatrixXd ret;
};