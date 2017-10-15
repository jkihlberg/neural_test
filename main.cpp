#include <iostream>
#include "network.hpp"
#include <vector>
#include <list>
#include <math.h>
#include <stdlib.h>

using namespace std;

// x
// y
// 1.0

// x && y
// x || y
// x
// y
// x xor y
// x xnor y
// x nand y
// x nor y

static double getError(vector<double> &inputs, vector<double> &outputs)
{
	double tmpError = 0;
	double accError = 0;
	bool x = inputs[0] == 1;
	bool y = inputs[1] == 1;
	bool xyxor = (inputs[0] + inputs[1]) == 1;

	tmpError = (x && y) ? fabs(outputs[0] - 1) : fabs(outputs[0]);
	accError += tmpError * tmpError;
	tmpError = (x || y) ? fabs(outputs[1] - 1) : fabs(outputs[1]);
	accError += tmpError * tmpError;
	tmpError = (x) ? fabs(outputs[2] - 1) 		: fabs(outputs[2]);
	accError += tmpError * tmpError;
	tmpError = (y) ? fabs(outputs[3] - 1) 		: fabs(outputs[3]);
	accError += tmpError * tmpError;
	tmpError = (xyxor) ? fabs(outputs[4] - 1) : fabs(outputs[4]);
	accError += tmpError * tmpError;
	tmpError = (xyxor == false) ? fabs(outputs[5] - 1) : fabs(outputs[5]);
	accError += tmpError * tmpError;
	tmpError = ((x && y) == false) ? fabs(outputs[6] - 1) 		: fabs(outputs[6]);
	accError += tmpError * tmpError;
	tmpError = ((x || y) == false) ? fabs(outputs[7] - 1) 		: fabs(outputs[7]);
	accError += tmpError * tmpError;

	return accError;
}

static void printValues(vector<double> &inputs, vector<double> &outputs)
{
	return;
	cout << inputs[0] << ' ';
	cout << inputs[1] << ' ';
	cout << outputs[0] << ' ';
	cout << outputs[1] << ' ';
	cout << outputs[2] << ' ';
	cout << outputs[3] << ' ';
	cout << outputs[4] << ' ';
	cout << outputs[5] << ' ';
	cout << outputs[6] << ' ';
	cout << outputs[7] << ' ';
	cout << "Error: " << getError(inputs, outputs);
	cout << endl;
}

vector<double> inputs;
vector<double> outputs;

void networkCompute(network &net)
{
	double error = 0;
	inputs[0] = 0.0;
	inputs[1] = 0.0;
	inputs[2] = 1.0;

	net.compute(inputs, outputs);
	printValues(inputs, outputs);
	error += getError(inputs, outputs);

	inputs[0] = 1.0;
	inputs[1] = 0.0;
	inputs[2] = 1.0;

	net.compute(inputs, outputs);
	printValues(inputs, outputs);
	error += getError(inputs, outputs);

	inputs[0] = 0.0;
	inputs[1] = 1.0;
	inputs[2] = 1.0;

	net.compute(inputs, outputs);
	printValues(inputs, outputs);
	error += getError(inputs, outputs);

	inputs[0] = 1.0;
	inputs[1] = 1.0;
	inputs[2] = 1.0;

	net.compute(inputs, outputs);
	printValues(inputs, outputs);
	error += getError(inputs, outputs);
//	cout << "Total error: " << error << endl;
	net.error = error;
}

int main()
{
	srand (time(NULL));

	rand();
	vector<network> networks;

	network net(3, 8);
	inputs.clear();
	inputs.push_back(0.0);
	inputs.push_back(0.0);
	inputs.push_back(0.0);

	for(int i = 0; i < 1000; i++){
		networks.push_back(net);
	}

	for(int generations = 0; generations < 1000; generations++){
		int initialNetworkSize = networks.size();

		for(int i = 0; i < initialNetworkSize; i++){
			for(int child = 0; child < 4; child++){
				networks.push_back(networks[i]);
//				networks[networks.size() - 1].mutate();
			}
		}

		for(int i = 0; i < networks.size(); i++){
			networks[i].mutate();
			networkCompute(networks[i]);
		}

		sort(networks.begin(), networks.end());
		networks.resize(initialNetworkSize);

		cout << generations << ' ';
		cout << networks[0].neurons.size() - 10 << ' ';
		for(int i = 0; i < 1; i++){
			cout << networks[i].error << ' ';
		}
		cout << endl;
	}

	return 0;
}
