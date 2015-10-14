	/*-----------------------------------------------------------------------------

	Copyright (C) 2005, 2006.

	A. Ronald Gallant
	Post Office Box 659
	Chapel Hill NC 27514-0659
	USA

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

	------------------------------------------------------------------------------

	Function     quantreg - Computes b for the regression y = Xb at quantile p.

	Syntax       #include "libscl.h"
	realmat quantreg(const realmat& y, const realmat& X, double p=0.5);

	Prototype in libsnp.h

	Description  y is an input n by 1 vector of dependent variables.  X is
	the n by K design matrix, p is the quantile, 0 < p < 1.

	Remark       Translation of the Matlab routine rq.m from Roger Koenker's 
	web site.  The rq.m code is at the end of this file.
              
	Regurn value  Returns b.

	Reference    Koenker, Roger, and Gibb Bassett (1978), "Regression
	Quantiles," Econometrica 46, 33-50.

	Functions    Library: sqrt
	called       libscl: intvec, realmat, ginv

	----------------------------------------------------------------------------*/

#include <Eigen/Dense>

using namespace Eigen;


template<typename M>
bool pinv(const M &a, 
				   M &result, 
				   double epsilon = std::numeric_limits<double>::epsilon())
{
	/*
	// Moore–Penrose pseudoinverse
	*/
	if(a.rows() < a.cols())
		return false;

	JacobiSVD< _Matrix_Type_ > svd = a.jacobiSvd(ComputeThinU | ComputeThinV);
	double tolerance = epsilon * std::max(a.cols(), a.rows()) * svd.singularValues().array().abs().maxCoeff();
	result = svd.matrixV() * ((svd.singularValues().array().abs() > tolerance).select(svd.singularValues().array().inverse(), 0)).matrix().asDiagonal() * svd.matrixU().adjoint();
}

RowVectorXd lp_fnm(const MatrixXd& A, const MatrixXd& TA, const RowVectorXd& c, const VectorXd& b, const VectorXd& u, VectorXd& x);

template<typename V>
V bound(const V& x, const V& dx);
  
VectorXd quantreg(const MatrixXd& X, const VectorXd& y, double p = 0.5);
