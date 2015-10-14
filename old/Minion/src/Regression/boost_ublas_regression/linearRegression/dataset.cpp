#include "dataset.h"

namespace  Dataset{

	double Train::randDouble(double min, double max)
	{
	    const long max_rand = 1000000L;
	    double x;
	    x = min + ( max - min) * (random() % max_rand) / max_rand;
	    return x;
	}
	
	void Train::createDataset (unsigned int nElement, 
						double xMin, double xMax,
						double yMin, double yMax) {
	  
	    srandom(time(NULL));
	    
	    // mat has nElement like a0 + a1x1 + a2x2
	    for (unsigned i = 0; i < nElement; ++ i){
			__space (i, 0) = 1;                       // a0, x0=1
			__space (i, 1) = randDouble(xMin, xMax);  // a1, x1
			__space (i, 2) = randDouble(xMin, xMax);  // a2, x2
			__class (i) = (rand() % 2);            // put a label 0/1, class
		}
	}
};
