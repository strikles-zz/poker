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

	VectorXd quantreg(const realmat& y, const realmat& X, double p=0.5);

	Description:	y is an input n by 1 vector of dependent variables.  
					X is the n by K design matrix, 
					p is the quantile, 0 < p < 1.

	Regurn value  Returns b.

	Remark       Translation of the Matlab routine rq.m from Roger Koenker's web site.  
				The rq.m code is at the end of this file.

	Reference    Koenker, Roger, and Gibb Bassett (1978), "Regression Quantiles," Econometrica 46, 33-50.

	----------------------------------------------------------------------------*/
#include <vector>

#include "Regression/CQuantileRegression.h"
#include "Util/Util_logging.h"

VectorXd quantreg(const MatrixXd& X, // X is the n by K design matrix
				  const VectorXd& y, // y is an input n by 1 vector of dependent variables.
				  double p)
{

	/*
		Computes b for the regression y = Xb at quantile p.
		Construct the dual problem of quantile regression
		Solve it with lp_fnm
	*/
  
	int m = X.rows();
	int n = X.cols();
  
#ifdef _DEBUG
	if (y.rows() != m) 
		gLog.WriteLog(eSeverityError, eCatData, "Error, quantreg, rows of y != rows of X");
	if (y.cols() != 1) 
		gLog.WriteLog(eSeverityError, eCatData, "Error, quantreg, cols of y != 1");
	if (m < n) 
		gLog.WriteLog(eSeverityError, eCatData, "Error, quantreg, X has more columns than rows");
	if (p <= 0 || p >=1) 
		gLog.WriteLog(eSeverityError, eCatData, "Error, quantreg, p not between 0 and 1");
#endif

	VectorXd u = VectorXd::Constant(m, 1.0);
	VectorXd a = VectorXd::Constant(m, (1.0 - p));
  
	MatrixXd TX = X.transpose();
	RowVectorXd Ty = -y.transpose();
	VectorXd TXa = TX*a;
  
	RowVectorXd Tb = lp_fnm(TX, X, Ty, TXa, u, a);
	VectorXd b = -Tb.transpose();
  
	return b;
}
  
  
RowVectorXd lp_fnm(const MatrixXd& A, const MatrixXd& TA, const RowVectorXd& c, const VectorXd& b, const VectorXd& u, VectorXd& x)
{
	/*
		Solve a linear program by the interior point method:
		min(c * u), s.t. A * x = b and 0 < x < u
		An initial feasible solution has to be provided as x
     
		Function lp_fnm of Daniel Morillo & Roger Koenker
		Translated from Ox to Matlab by Paul Eilers 1999
		Modified by Roger Koenker 2000--
		More changes by Paul Eilers 2004
		Translated from Matlab to C++ by A. Ronald Gallant 2005
	*/
    
	/*
		A  is m by n    y  is 1 by m    AQ is m by n    fx is n by 1
		c  is 1 by n    r  is 1 by n    dy is 1 by m    fs is n by 1
		b  is m by 1    z  is 1 by n    dx is n by 1    fw is 1 by n
		u  is n by 1    w  is 1 by n    ds is n by 1    fz is 1 by n
		x  is n by 1    q  is n by 1    dz ia 1 by n    fp is scalar
		s  is n by 1    Q  is n by n    dw is 1 by n    fd is scalar
	*/
    
	// Set some constants
	double beta		= 0.9995;
	double small	= 1.0e-5;
	int max_it		= 50;

	int m = A.rows();
	int n = A.cols();
    
	// Generate inital feasible point
	VectorXd s = u - x;
	VectorXd Tc = c.transpose();
	MatrixXd TA_pinv = TA.inverse();
	//pinv(TA, TA_pinv);
	RowVectorXd y = (TA_pinv*Tc).transpose();
    
	RowVectorXd r = c - y*A;
	RowVectorXd z = r;
	for (int i = 1; i <= n; ++i) 
	{
		if (r[i] == 0.0) 
			r[i] += 0.001;
		if (r[i] <= 0.0) 
			z[i] = 0.0;
	}

	RowVectorXd w = z - r;
	VectorXd gap = c*x - y*b + w*u;
    
	// Start iterations 
	int it = 0;
	while (gap(1) > small && it < max_it) 
	{
		++it;
        
		// Compute affine step
		VectorXd q = VectorXd::Zero(n);
		for (int i=1; i <= n; ++i) 
			q[i] = 1.0 / (z[i]/x[i] + w[i]/s[i]);

		r = z - w;
  
		MatrixXd TAQ = MatrixXd::Zero(n,m);
		for (int j=1; j <= m; ++j) 
		{
			for (int i=1; i <= n; ++i) 
				TAQ(i,j) = TA(i,j)*std::sqrt(q[i]);
		}
		
		// fix me
		MatrixXd gTAQ = TAQ.inverse();
		//pinv(TAQ, gTAQ);
  
		VectorXd rhs = r.transpose();
		for (int i=1; i<=n; ++i) 
			rhs[i] *= std::sqrt(q[i]); 
		RowVectorXd dy = (gTAQ*rhs).transpose();

		VectorXd dx = (dy*A - r).transpose();
		for (int i=1; i <= n; ++i) 
			dx[i] *= q[i];
		VectorXd ds = -dx;

		RowVectorXd dz = RowVectorXd::Zero(n);
		RowVectorXd dw = RowVectorXd::Zero(n);
		for (int i=1; i <= n; ++i) 
		{
			dz[i] = -z[i]*(1.0 + dx[i]/x[i]);
			dw[i] = -w[i]*(1.0 + ds[i]/s[i]);
		}
        
		// Compute maximum allowable step lengths
		VectorXd fx = bound(x, dx);
		VectorXd fs = bound(s, ds);
		RowVectorXd fw = bound(w, dw);
		RowVectorXd fz = bound(z, dz);
  
		VectorXd v = beta*fs;
		for (int i=1; i <= n; ++i)
		{
			if (fx[i] < fs[i]) 
				v[i] = beta*fx[i];
		}

		double fp = 1.0;
		for (int i=1; i<= n; ++i) 
		{
			if (v[i] < fp) 
				fp = v[i];
		}
        
		v = beta * fz; 
		for (int i=1; i <= n; ++i) 
		{
			if (fw[i] < fz[i]) 
				v[i] = beta*fw[i];
		}

		double fd = 1.0;
		for (int i=1; i <= n; ++i) 
		{
			if (v[i] < fd) 
				fd = v[i];
		}
  
		// If full step is feasible, take it. Otherwise modify it
		if ( (fp < fd ? fp : fd) < 1) 
		{
			// Update mu
			// dafuq is this ?????????
			double mu = (z*x + w*s)[1];
			double g = ((z + fd*dz)*(x + fp*dx) + (w + fd*dw)*(s + fp*ds))[1];
			mu *= std::pow(g/mu,3) / double(2*n);
  
			//Compute modified step
			VectorXd dxdz = VectorXd::Zero(n); 
			for (int i=1; i <= n; ++i) 
				dxdz[i] = dx[i] * dz[i];

			VectorXd dsdw = VectorXd::Zero(n);
			for (int i=1; i <= n; ++i) 
				dsdw[i] = ds[i] * dw[i];

			VectorXd xinv = VectorXd::Zero(n);
			for (int i=1; i <= n; ++i) 
				xinv[i] = 1.0 / x[i];

			VectorXd sinv = VectorXd::Zero(n);
			for (int i=1; i <= n; ++i) 
				sinv[i] = 1.0 / s[i];

			VectorXd xi = mu * (xinv - sinv);
			VectorXd drhs = dxdz - dsdw - xi;

			for (int i=1; i <= n; ++i) 
				drhs[i] *= std::sqrt(q[i]);; 

			rhs += drhs;
			dy = (gTAQ*rhs).transpose();
			dx = (dy*A).transpose() + xi - r.transpose() - dxdz + dsdw;

			for (int i=1; i <= n; ++i) 
				dx[i] *= q[i];

			ds = -dx;
			for (int i=1; i <= n; ++i)
			{
				dz[i] = mu*xinv[i] - z[i] - xinv[i]*z[i]*dx[i] - dxdz[i];
				dw[i] = mu*sinv[i] - w[i] - sinv[i]*w[i]*ds[i] - dsdw[i];
			}
            
			// Compute maximum allowable step lengths
			fx = bound(x, dx);
			fs = bound(s, ds);
			fw = bound(w, dw);
			fz = bound(z, dz);
          
			v = beta*fs;
			for (int i=1; i<=n; ++i) 
			{
				if (fx[i]<fs[i]) 
					v[i]=beta*fx[i];
			}

			fp = 1.0;
			for (int i=1; i<=n; ++i) 
			{
				if (v[i] < fp) 
					fp = v[i];
			}
        
			v = beta*fz; 
			for (int i=1; i<=n; ++i) 
			{
				if (fw[i]<fz[i]) 
					v[i]=beta*fw[i];
			}

			fd = 1.0;
			for (int i=1; i<=n; ++i) 
			{
				if (v[i] < fd) 
					fd = v[i];
			}
  
		}
        
		// Take the step 
		x += fp*dx;
		s += fp*ds;
		y += fd*dy;
		w += fd*dw;
		z += fd*dz;
  
		gap = c*x - y*b + w*u;
	}
  
	return y;
}

template<typename V>
V bound(const V& x, const V& dx)
{
	/*
		Fill vector with allowed step lengths
		Support function for lp_fnm
	*/

	int n = x.cols(); 
	V b = V::Constant(n, 1.0e20);

	std::vector<int> f;
	for (int i=1; i<=n; ++i) 
	{
		if (dx[i] < 0.0) 
			f.push_back(i); 
	}

	int m = f.size();
	for (int i=1; i<=m; ++i) 
		b(f[i]) = -x(f[i]) / dx(f[i]);

	return b;
}
