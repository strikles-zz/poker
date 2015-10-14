#include "dataset.h"
#include "linearRegr.h"

int main(){

	unsigned int numElements = 10;
	
	Dataset::Train trainSpace(numElements); 
	std::cerr << " Created train space";
	trainSpace.createDataset(numElements, -100.00, 100.0, -200.0, 200.0);	
	std::cerr << " created dataset";	
    linearRegressionDescGrad(0.1, 50, trainSpace); 
}
