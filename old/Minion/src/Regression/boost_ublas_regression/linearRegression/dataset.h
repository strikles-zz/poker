#ifndef DATASET_H
#define DATASET_H 1
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>
#include <time.h>

//
// Storage for Training dataset
//
namespace Dataset{

	using namespace boost::numeric::ublas;
	
	typedef matrix<double> Mat;

	class Train{
				
	private:	
		matrix<double>  __space;  // representing xi in a0 + a1x1 + a2x2 
		vector<unsigned short>  __class;  // representing the class 0/1 
		double randDouble(double min, double max);
	    
	public:				
		Train(unsigned int numElements) { 
			__space.resize(numElements, 3);  // a0, a1, a2 for this code
			__class.resize(numElements);     // numElement instances
		}
		inline Mat & data() { return __space; } 
		inline vector<unsigned short> & labels() { return __class; } 
		
		//
		// create a dataset with nElements, each xi is in [xMin, xMax]
		//
		void createDataset (unsigned int nElement, 
							double xMin, double xMax,
							double yMin, double yMax);
	};
		
};					 
#endif
