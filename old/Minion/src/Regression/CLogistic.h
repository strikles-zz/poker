#pragma once

//#include <unsupported/Eigen/NonLinearOptimization>

#include "DB/CData.h"


class CLogistic
{
public:
	CLogistic() {};
	CLogistic(int br_ndx, int nacted, eBetType action);
	CLogistic(Matrix<double, Dynamic, Dynamic, RowMajor>& X, VectorXd& y, VectorXd& theta) {};

public:
	VectorXd J_history;
	Matrix<double, Dynamic, Dynamic, RowMajor> classifier;

public:
	double	sigmoid(const double& x);
	void	TrainOnevsAll(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, int num_labels, double lambda);
	int		PredictOnevsAll(const VectorXd& point);
	double	Cost(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, VectorXd& theta, double lambda);
	void	GradientDescent(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y_class, VectorXd& theta, double alpha);

private:

	int m_npoints, m_ndims;
	CDataObject* m_data;

private:
	bool init();
};