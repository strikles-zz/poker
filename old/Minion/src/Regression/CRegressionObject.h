#pragma once

//#include <unsupported/Eigen/NonLinearOptimization>

#include "DB/CData.h"


class CRegressionObject
{
public:
	CRegressionObject() {};
	CRegressionObject(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y);

public:
	double	Predict(const RowVectorXd& point);
	void	Train(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, VectorXd& theta, double lambda);
	double	Cost(const RowVectorXd& point,const double y, VectorXd& theta);
	void	GradientDescent(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, VectorXd& theta, double lambda);
	VectorXd get_theta() const { return m_theta; }
	
	bool ColPivSolver(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, VectorXd& theta);
	bool SVDSolver(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, VectorXd& theta);

private:
	VectorXd m_theta, J_history;
	double m_lambda;
};