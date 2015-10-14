#include <iostream>
#include <math.h>
#include "HiddenLayer.h"
#include "RBM.h"
#include "LogisticRegression.h"
#include "DBN.h"

#include "utils.h"

using namespace std;


// DBN
DBN::DBN(int size, int n_i, int *hls, int n_o, int n_l) 
{
	int input_size;
	N = size;
	n_ins = n_i;
	hidden_layer_sizes = hls;
	n_outs = n_o;
	n_layers = n_l;

	sigmoid_layers = new HiddenLayer*[n_layers];
	rbm_layers = new RBM*[n_layers];

	// construct multi-layer
	for(int i=0; i<n_layers; i++) 
	{
		if(i == 0)
		{
			input_size = n_ins;
		} 
		else 
		{
			input_size = hidden_layer_sizes[i-1];
		}
		// construct sigmoid_layer
		sigmoid_layers[i] = new HiddenLayer(N, input_size, hidden_layer_sizes[i], NULL, NULL);
		// construct rbm_layer
		rbm_layers[i] = new RBM(N, input_size, hidden_layer_sizes[i],sigmoid_layers[i]->W, sigmoid_layers[i]->b, NULL);
	}

	// layer for output using LogisticRegression
	log_layer = new LogisticRegression(N, hidden_layer_sizes[n_layers-1], n_outs);
}

DBN::~DBN() 
{
	delete log_layer;
	for(int i=0; i<n_layers; i++)
	{
		delete sigmoid_layers[i];
		delete rbm_layers[i];
	}
	delete[] sigmoid_layers;
	delete[] rbm_layers;
}


void DBN::pretrain(int *input, double lr, int k, int epochs)
{
	int *layer_input;
	int prev_layer_input_size;
	int *prev_layer_input;
	int *train_X = new int[n_ins];

	// layer-wise
	for(int i=0; i<n_layers; i++) 
	{
		// training epochs
		for(int epoch=0; epoch<epochs; epoch++) 
		{
			// input x1...xN
			for(int n=0; n<N; n++) 
			{
				// initial input
				for(int m=0; m<n_ins; m++) 
					train_X[m] = input[n * n_ins + m];

				// layer input
				for(int l=0; l<=i; l++) 
				{
					if(l == 0) 
					{
						layer_input = new int[n_ins];
						for(int j=0; j<n_ins; j++) 
							layer_input[j] = train_X[j];
					}
					else
					{
						if(l == 1)
							prev_layer_input_size = n_ins;
						else
							prev_layer_input_size = hidden_layer_sizes[l-2];

						prev_layer_input = new int[prev_layer_input_size];
						for(int j=0; j<prev_layer_input_size; j++)
							prev_layer_input[j] = layer_input[j];
							
						delete[] layer_input;
						layer_input = new int[hidden_layer_sizes[l-1]];
						sigmoid_layers[l-1]->sample_h_given_v(prev_layer_input, layer_input);
						delete[] prev_layer_input;
					}
				}

				rbm_layers[i]->contrastive_divergence(layer_input, lr, k);
			}
		}
	}

	delete[] train_X;
	delete[] layer_input;
}

void DBN::finetune(int *input, int *label, double lr, int epochs) 
{
	int *layer_input;
	int *prev_layer_input;

	int *train_X = new int[n_ins];
	int *train_Y = new int[n_outs];

	for(int epoch=0; epoch<epochs; epoch++) 
	{
		// input x1...xN
		for(int n=0; n<N; n++) 
		{ 
			// initial input
			for(int m=0; m<n_ins; m++)
				train_X[m] = input[n * n_ins + m];
			for(int m=0; m<n_outs; m++)
				train_Y[m] = label[n * n_outs + m];

			// layer input
			for(int i=0; i<n_layers; i++) 
			{
				if(i == 0) 
				{
					prev_layer_input = new int[n_ins];
					for(int j=0; j<n_ins; j++) 
						prev_layer_input[j] = train_X[j];
				} 
				else 
				{
					prev_layer_input = new int[hidden_layer_sizes[i-1]];
					for(int j=0; j<hidden_layer_sizes[i-1]; j++) 
						prev_layer_input[j] = layer_input[j];
					delete[] layer_input;
				}

				layer_input = new int[hidden_layer_sizes[i]];
				sigmoid_layers[i]->sample_h_given_v(prev_layer_input, layer_input);
				delete[] prev_layer_input;
			}
			log_layer->train(layer_input, train_Y, lr);
		}
	}

	delete[] layer_input;
	delete[] train_X;
	delete[] train_Y;
}

void DBN::predict(int *x, double *y) 
{
	double *layer_input;
	double *prev_layer_input;
	double linear_output;

	prev_layer_input = new double[n_ins];
	for(int j=0; j<n_ins; j++) 
		prev_layer_input[j] = x[j];

	// layer activation
	for(int i=0; i<n_layers; i++)
	{
		layer_input = new double[sigmoid_layers[i]->n_out];
		for(int k=0; k<sigmoid_layers[i]->n_out; k++) 
		{
			linear_output = 0.0;
			for(int j=0; j<sigmoid_layers[i]->n_in; j++)
			{
				linear_output += sigmoid_layers[i]->W[k][j] * prev_layer_input[j];
			}
			linear_output += sigmoid_layers[i]->b[k];
			layer_input[k] = sigmoid(linear_output);
		}
		delete[] prev_layer_input;

		if(i < n_layers-1) 
		{
			prev_layer_input = new double[sigmoid_layers[i]->n_out];
			for(int j=0; j<sigmoid_layers[i]->n_out; j++) 
				prev_layer_input[j] = layer_input[j];
			delete[] layer_input;
		}
	}

	for(int i=0; i<log_layer->n_out; i++) 
	{
		y[i] = 0;
		for(int j=0; j<log_layer->n_in; j++) 
		{
			y[i] += log_layer->W[i][j] * layer_input[j];
		}
		y[i] += log_layer->b[i];
	}

	log_layer->softmax(y);

	delete[] layer_input;
}

void test_dbn()
{
	srand(0);

	double pretrain_lr = 0.1;
	int pretraining_epochs = 1000;
	int k = 1;
	double finetune_lr = 0.1;
	int finetune_epochs = 500;
	int train_N = 6;
	int test_N = 3;
	int n_ins = 6;
	int n_outs = 2;
	int hidden_layer_sizes[] = {3, 3};
	int n_layers = sizeof(hidden_layer_sizes) / sizeof(hidden_layer_sizes[0]);

	// training data
	int train_X[6][6] = {
		{1, 1, 1, 0, 0, 0},
		{1, 0, 1, 0, 0, 0},
		{1, 1, 1, 0, 0, 0},
		{0, 0, 1, 1, 1, 0},
		{0, 0, 1, 1, 0, 0},
		{0, 0, 1, 1, 1, 0}
	};

	int train_Y[6][2] = {
		{1, 0},
		{1, 0},
		{1, 0},
		{0, 1},
		{0, 1},
		{0, 1}
	};

	// construct DBN
	DBN dbn(train_N, n_ins, hidden_layer_sizes, n_outs, n_layers);
	// pretrain
	dbn.pretrain(*train_X, pretrain_lr, k, pretraining_epochs);
	// finetune
	dbn.finetune(*train_X, *train_Y, finetune_lr, finetune_epochs);

	// test data
	int test_X[3][6] = {
		{1, 1, 0, 0, 0, 0},
		{0, 0, 0, 1, 1, 0},
		{1, 1, 1, 1, 1, 0}
	};

	double test_Y[3][2];

	// test
	for(int i=0; i<test_N; i++) 
	{
		dbn.predict(test_X[i], test_Y[i]);
		for(int j=0; j<n_outs; j++)
		{
			cout << test_Y[i][j] << " ";
		}
		cout << endl;
	}
}