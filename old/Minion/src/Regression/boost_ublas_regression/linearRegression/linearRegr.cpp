#include "linearRegr.h"


vector<double> linearRegressionDescGrad(double alpha, unsigned int numIterations,
					Dataset::Train & t){

	vector<double> weigths(t.data().size2());  // learnt a0, a1, a2 (n=3 x 1)
	vector<double> h(t.data().size1());		   // h  (m x 1)
	vector<double> err(t.data().size1());	   // err (m x 1)
	
	for (unsigned int i = 0; i<weigths.size(); i++)  // assign 1.0
		weigths(i) = 1.0;
		
	for (unsigned int i = 0; i < numIterations; i++){  // for num iteration
		h = prod(t.data(), weigths);   // m x 1 = (m x 3) * (3 x 1)
		std::transform(h.begin(), h.end(),  // apply sigmoid
					   h.begin(), sigmoid) ;
		
		err = h - t.labels();  // compute new err
		weigths =				// update weights
			weigths + alpha * prod (trans ( t.data() ) , err);
	}
	return weigths;
}


vector<double> linearRegressionStocDescGrad(double alpha, unsigned int numIterations,
					Dataset::Train & t){

	vector<double> weigths(t.data().size2());  // learnt a0, a1, a2 (n=3 x 1)
	double h;		   						   // h  (m x 1)
	double e;	   							   // err (m x 1)
	
	for (unsigned int i = 0; i<weigths.size(); i++)  // assign 1.0
		weigths(i) = 1.0;
		
	// in this case h and err	
	for (unsigned int i = 0; i < t.data().size1(); i++){  // for numElements
		matrix_row<matrix<double> > mr (t.data(), i);     // get the row i
		h = inner_prod(mr, weigths);
		h = sigmoid(h);
		e = t.labels()(i) - h;
		weigths =				// update weights
			weigths + alpha * e * mr;
	}
	return weigths;
}
