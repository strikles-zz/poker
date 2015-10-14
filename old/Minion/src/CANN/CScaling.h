#pragma once

#include "Config/CPreferences.h"

class CScaling
{

public:

	static int get_dim_scale_preflop(int j, int value);			//{ return gPrefs.knn.dim_scale_preflop[j] * dynamic_metric(j, value); }
	static int get_dim_scale_postflop(int j, int value);		//{ return gPrefs.knn.dim_scale_postflop[j] * dynamic_metric(j, value); }
	static int dynamic_metric_pre(int j, int value);
	static int dynamic_metric_post(int j, int value);

private:
	static unsigned int util(unsigned int value)
	{
		unsigned int count = 0, i = 1;
		while (i < value)
		{
			i <<= 1;
			count++;
		}

		return count;
	}

	/*
	static int EigenTest()
	{
		// Eigen Test
		for (int size=1; size<=4; ++size)
		{
			MatrixXi m(size,size+1); // a (size)x(size+1)-matrix of int's
			for (int j=0; j<m.cols(); ++j) // loop over columns
				for (int i=0; i<m.rows(); ++i) // loop over rows
					m(i,j) = i+j*m.rows(); // to access matrix coefficients,
			// use operator()(int,int)
			std::cout << m << "\n\n";
		}
		VectorXf v(4); // a vector of 4 float's
		// to access vector coefficients, use either operator () or operator []
		v[0] = 1; v[1] = 2; v(2) = 3; v(3) = 4;
		std::cout << "\nv:\n" << v << std::endl;
		// End Test

		return 0;
	}
	*/
};
