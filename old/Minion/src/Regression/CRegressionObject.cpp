#include <iostream>

#include "Regression/CRegressionObject.h"

#include "Master/CGlobalVars.h"
#include "Util/Util_logging.h"


CRegressionObject::CRegressionObject(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y)
{
	// Initialize our weights vector
	m_theta = VectorXd::Zero(X.cols());
	// Set a penalization value
	m_lambda = 0.0001;

	Train(X, y, m_theta, m_lambda);
}

void CRegressionObject::Train(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, VectorXd& theta, double lambda)
{
	// Split dataset 
	// Empty

	SVDSolver(X, y, m_theta);

	// norm() is L2 norm
	double relative_error = (X*theta - y).norm() / y.norm(); 

	/*
	if(relative_error > 1.0)
		GradientDescent(X, y, m_theta, m_lambda);
	*/

	cout << "The relative error is:\n" << relative_error << endl;

	std::cout << "Theta:" << m_theta << std::endl;

	// Cross validation
	// Empty
}

double CRegressionObject::Predict(const RowVectorXd& point)
{
	double prediction = point * m_theta;
	return prediction;
}

bool CRegressionObject::SVDSolver(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, VectorXd& theta)
{
	theta = X.jacobiSvd(ComputeThinU | ComputeThinV).solve(y);

	return true;
}

void CRegressionObject::GradientDescent(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, VectorXd& theta, double lambda)
{
	/*
	// Performs stochastic gradient descent to learn theta
	*/

	gLog.WriteLog(eSeverityInfo, eCatPerformance, "Starting Stochastic Gradient Descent\n");
	J_history = VectorXd::Zero(X.rows());
	for(int iter = 0; iter < X.rows(); iter++)
	{
		double prediction = X.row(iter) * theta;
		double error = prediction - y(iter);
		// update theta
		theta -= (lambda * error * X.row(iter));
		// Save the cost J in every iteration    
		J_history(iter) = Cost(X.row(iter), y(iter), theta);
		//gLog.WriteLog(eSeverityInfo, eCatPerformance, "Cost = %6.6lf\n", J_history(iter));
	}
	gLog.WriteLog(eSeverityInfo, eCatPerformance, "Done :)\n");
}

double CRegressionObject::Cost(const RowVectorXd& point,const double y, VectorXd& theta)
{
	double J = (point*theta - y);
	J *= J/2.0;

	return J;
}