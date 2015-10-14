#include <iostream>
#include <functional>

#include "Regression/CLogistic.h"

#include "Master/CGlobalVars.h"
#include "Util/Util_logging.h"




CLogistic::CLogistic(int br_ndx, int nacted, eBetType action)
{
	m_data = gVars.GetDataObject(br_ndx, nacted, action);
	if(m_data)
	{
		m_npoints = m_data->get_npoints();
		m_ndims = m_data->get_ndims();
	}
}

double CLogistic::sigmoid(const double& x)
{
	return 1.0 / (1.0 + exp(-x));
}

void CLogistic::TrainOnevsAll(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y_class, int num_labels, double lambda)
{
	/*
		trains multiple logistic regression classifiers and returns all
		the classifiers in a matrix classifier, where the i-th row of classifier 
		corresponds to the classifier for label i
	*/

	int m = X.rows();
	int n = X.cols();
	classifier = Matrix<double, Dynamic, Dynamic, RowMajor>::Zero(num_labels, n);

	// Iterate through all the classification classes
	for(int class_ndx = 0; class_ndx < num_labels; class_ndx++)
	{
		VectorXd theta  = VectorXd::Zero(n);

		// classify one vs. all
		VectorXd c = VectorXd::Zero(y_class.rows());
		for (int point_ndx = 0; point_ndx < y_class.rows(); point_ndx++)
			c(class_ndx) = (y_class(point_ndx) == class_ndx ? 1.0 : .0);

		GradientDescent(X, c, theta, lambda);

		// store the result inside classifier
		classifier.row(class_ndx) = theta.transpose();
	}
}

int CLogistic::PredictOnevsAll(const VectorXd& point)
{

	// compute prediction for one vs. all
	VectorXd prob = (point * classifier.transpose());

	// pick the most expressive
	return prob.maxCoeff();
}

void CLogistic::GradientDescent(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y_class, VectorXd& theta, double lambda)
{
	/*
	// Performs stochatic gradient descent to learn theta
	*/
	double prediction, hypothesis, error;
	for(int iter = 0; iter < X.rows(); iter++)
	{
		prediction = X.row(iter) * theta;
		hypothesis = sigmoid(prediction);
		error = hypothesis - y_class(iter);
		theta -= (lambda * error) * X.row(iter);
	}
}

double CLogistic::Cost(const Matrix<double, Dynamic, Dynamic, RowMajor>& X, const VectorXd& y, VectorXd& theta, double lambda)
{
	/*
		computes the cost of using theta as the parameter for regularized logistic regression 
		and the gradient of the cost w.r.t. to the parameters. 
	*/
	
	/*
	int m = X.rows();

	// compute cost
	VectorXd h_theta = (X * theta).unaryExpr(std::mem_fun(&CLogistic::sigmoid));

	double reg_param = (theta.transpose() * theta);
	reg_param *= (lambda/(2*m));

	VectorXd log_h_theta = (h_theta.array()).log();
	double cost = (y.transpose() * log_h_theta);
	cost += (RowVectorXd::Constant(y.rows(), 1.0) - y.transpose()) * (VectorXd::Constant(h_theta.rows(), 1.0) - log_h_theta);
	
	double J = - (1/m * cost) + reg_param;

	// PARTIAL DERIVATIVES
	// compute the usual partial derivatives, without regularization
	VectorXd pd = 1/m * (X.transpose() * (h_theta - y));
	VectorXd pd_reg_param = (lambda/m) * theta;

	// apply regularization, except for theta = 1 so set first term = 0
	pd_reg_param(0) = 0;
	VectorXd grad = VectorXd::Zero(theta.rows());
	grad = pd + pd_reg_param;

	return J;
	*/
	return 0;
}