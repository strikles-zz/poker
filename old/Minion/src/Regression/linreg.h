/*
    Linear Regression with one or multiple variables
    Author: Kan Ouivirach
    Credit: David C. Swaim II, Ph.D.
*/
#pragma once


#include <string>
#include <math.h>

#include "DB/CData.h"

using namespace std;


class LinearRegression 
{
public:
    LinearRegression(int br_ndx, int nacted, eBetType action);
    ~LinearRegression();

public:    
    RowVectorXd get_theta();
    VectorXd get_mean();
    VectorXd get_std();

	void Regression();    
    void feature_normalize();
    double compute_cost(Matrix<double, Dynamic, Dynamic, RowMajor>& X, VectorXd& y, RowVectorXd& theta);
	void gradient_descent(double alpha, int num_iters, bool norm);

private:
	// the data
    CDataObject* m_data;
	// stats
    VectorXd m_mean;
    VectorXd m_std;
    VectorXd m_predicted_data;
    RowVectorXd m_theta;
};

