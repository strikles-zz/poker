#include "linreg.h"

#include "Master/CGlobalVars.h"


LinearRegression::LinearRegression(int br_ndx, int nacted, eBetType action)
{
	m_data = gVars.GetDataObject(br_ndx, nacted, action);
	m_theta = VectorXd::Zero(m_data->m_points.cols());
}

LinearRegression::~LinearRegression() 
{
}

double LinearRegression::compute_cost(Matrix<double, Dynamic, Dynamic, RowMajor>& X, VectorXd& y, RowVectorXd& theta)
{
    double J = 0;
    unsigned int m = y.rows();

    VectorXd predictions = VectorXd::Zero(X.rows());
    for(unsigned int i = 0; i < X.rows(); i++) 
	{
		double val = X.row(i).dot(theta.transpose());
        predictions(i) = val;
    }

    VectorXd errors = VectorXd::Zero(predictions.rows());
    for(unsigned int i = 0; i < predictions.rows(); i++) 
	{
        double diff = predictions(i) - y(i);
        errors(i) = diff;
    }

    J = (1.0 / (2 * m)) * errors.dot(errors.transpose());

    return J;
}

void LinearRegression::gradient_descent(double alpha, int num_iters, bool norm) 
{
    
    unsigned int m = m_predicted_data.rows();

    // initialize theta
    for(unsigned int i = 0; i < m_data->m_points.cols(); i++) 
	{
        m_theta(i) = 0;
    }

    double J = 0;
	VectorXd J_history = VectorXd::Zero(num_iters);
    for(int iter = 0; iter < num_iters; iter++) 
	{
        // predict first
        VectorXd predictions = VectorXd::Zero(m_data->m_points.rows());
        for(unsigned int i = 0; i < m_data->m_points.rows(); i++) 
		{
            double val = m_data->m_points.row(i).dot(m_theta.transpose());
            predictions(i) = val;
        }

        // then compute the error and update the theta
        double error = 0;
        for(unsigned int i = 0; i < m_theta.cols(); i++) 
		{
            VectorXd diff = VectorXd::Zero(m_data->m_points.cols());
            VectorXd X_col = VectorXd::Zero(m_data->m_points.cols());
            for(unsigned int j = 0; j < m_data->m_points.rows(); j++) 
			{
                diff(i)		= predictions(j) - m_predicted_data(j);
                X_col(i)	= m_data->m_points(j,i);
            }
            error = diff.dot(X_col.transpose());

            // update the theta
            m_theta(i) -= (alpha * (1.0 / m) * error);
        }
        
        J = compute_cost(m_data->m_points, m_predicted_data, m_theta);
        J_history(iter) = J;
    }
}

RowVectorXd LinearRegression::get_theta() 
{
    return m_theta;
}

VectorXd LinearRegression::get_mean() 
{
    return m_mean;
}

VectorXd LinearRegression::get_std() 
{
    return m_std;
}

void LinearRegression::Regression()
{
    double alpha = 0.001;
    int num_iters = 1500;
    bool norm = false;

    gradient_descent(alpha, num_iters, norm);
}

