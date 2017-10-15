#include <iostream>
#include "network.hpp"
#include <vector>
#include <list>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

using namespace std;
//xy 0,0  3,0
   // 0 1 2
   // 3 4 5
   // 6 7 8
// 0,3    3,3
static bool move(vector<double> &inputs, vector<double> &outputs)
{
	int idx = distance(outputs.begin(), max_element(outputs.begin(), outputs.end()));
	//cout << "Moving: " << idx << endl;

	if(inputs[idx] != 0.0){
		return false;
	}

	inputs[idx] = 1.0;
	return true;
}

static bool gameOver(vector<double> &inputs, int &winner)
{
	for(int i = 0; i < 3; i++){
		if(inputs[i * 3] == inputs[i * 3 + 1] && inputs[i * 3] == inputs[i * 3 + 2] && inputs[i * 3] != 0.0){
			winner = inputs[i * 3];
			return true;
		}

		if(inputs[0 * 3 + i] == inputs[1 * 3 + i] && inputs[0 * 3 + i] == inputs[2 * 3 + i] && inputs[0 * 3 + i] != 0.0){
			winner = inputs[0 * 3 + i];
			return true;
		}
	}

	if(inputs[0 * 3 + 0] == inputs[1 * 3 + 1] && inputs[0 * 3 + 0] == inputs[2 * 3 + 2] && inputs[0 * 3 + 0] != 0.0){
		winner = inputs[0 * 3 + 0];
		return true;
	}

	if(inputs[0 * 3 + 2] == inputs[1 * 3 + 1] && inputs[0 * 3 + 2] == inputs[2 * 3 + 0] && inputs[0 * 3 + 2] != 0.0){
		winner = inputs[0 * 3 + 2];
		return true;
	}

	for(int i = 0; i < 9; i++){
		if(inputs[i] == 0.0){
			return false;
		}
	}

	winner = 0;
	return true;
}

static void printValues(vector<double> &inputs, vector<double> &outputs)
{
	cout << "Inputs:" << endl;
	cout << inputs[0] << ' ';
	cout << inputs[1] << ' ';
	cout << inputs[2] << endl << endl;
	cout << inputs[3] << ' ';
	cout << inputs[4] << ' ';
	cout << inputs[5] << endl << endl;
	cout << inputs[6] << ' ';
	cout << inputs[7] << ' ';
	cout << inputs[8] << endl << endl << endl;

	cout << "Outputs:" << endl;
	cout << outputs[0] << ' ';
	cout << outputs[1] << ' ';
	cout << outputs[2] << endl << endl;
	cout << outputs[3] << ' ';
	cout << outputs[4] << ' ';
	cout << outputs[5] << endl << endl;
	cout << outputs[6] << ' ';
	cout << outputs[7] << ' ';
	cout << outputs[8] << endl << endl << endl;
}

void toggleInputs(vector<double> &inputs)
{
	for(auto &i:inputs){
		if(i == 1.0){
			i = 2.0;
		}
		else if(i == 2.0){
			i = 1.0;
		}
	}
}

void playGame(network &opp1, network &opp2)
{
	int winner;
	bool gameGoing = true;
	vector<double> inputs(9, 0.0);
	vector<double> outputs(9, 0.0);
	//inputs[9] = 1.0f;

	while(gameGoing){
		// Player 1
		opp1.compute(inputs, outputs);
		if(move(inputs, outputs) == false) {
			opp1.addError(4.0);
			gameGoing = false;
			continue;
		}
		toggleInputs(inputs);
		if(gameOver(inputs, winner)){
			if(winner != 0){
				opp1.addError(2.0);
			}
			gameGoing = false;
			continue;
		}

		// Player 2
		opp2.compute(inputs, outputs);
		if(move(inputs, outputs) == false) {
			opp1.addError(4.0);
			gameGoing = false;
			continue;
		}
		toggleInputs(inputs);
		if(gameOver(inputs, winner)){
			if(winner != 0){
				opp1.addError(2.0);
			}
			gameGoing = false;
			continue;
		}
	}
}

void playHuman(network &opp1)
{
	while(true){
		int winner;
		bool gameGoing = true;
		vector<double> inputs(9, 0.0);
		vector<double> outputs(9, 0.0);
		//inputs[9] = 1.0f;

		while(gameGoing){
			// Player 1
			opp1.compute(inputs, outputs);
			if(move(inputs, outputs) == false) {
				cout << "You win, opponent stupid!" << endl;
				gameGoing = false;
				continue;
			}
			toggleInputs(inputs);
			if(gameOver(inputs, winner)){
				if(winner != 0){
					cout << "You lose!" << endl;
				}
				else {
					cout << "Draw" << endl;
				}
				gameGoing = false;
				continue;
			}

			// Player 2
			printValues(inputs, inputs);
			cout << "Enter where to play: ";
			int userMove;
			cin >> userMove;
			cout << "You entered: " << userMove << endl;
			outputs[userMove] = 1000.0;
			if(move(inputs, outputs) == false) {
				cout << "Invalid move!" << endl;
				gameGoing = false;
				continue;
			}
			toggleInputs(inputs);
			if(gameOver(inputs, winner)){
				if(winner != 0){
					cout << "You win!" << endl;
				}
				else {
					cout << "Draw" << endl;
				}
				gameGoing = false;
				continue;
			}
		}
	}
}

int main()
{
	srand (time(NULL));

	rand();
	vector<network> networks;

	network net(9, 9);

	for(int i = 0; i < 100; i++){
		networks.push_back(net);
		net.mutate();
	}

	for(int generations = 0; generations < 1000; generations++){
		int initialNetworkSize = networks.size();

		for(int i = 0; i < initialNetworkSize; i++){
			networks[i].error = 0.0;
			for(int child = 0; child < 1; child++){
				networks.push_back(networks[i]);
//				networks[networks.size() - 1].mutate();
			}
		}

		for(int i = 0; i < networks.size(); i++){
			networks[i].mutate();
		}

		for(int opp1 = 0; opp1 < networks.size(); opp1++){
			for(int opp2 = 0; opp2 < networks.size(); opp2++){
				if(opp1 == opp2){
					continue;
				}

				playGame(networks[opp1], networks[opp2]);
			}
		}

		sort(networks.begin(), networks.end());
		networks.resize(initialNetworkSize);

		cout << generations << ' ';
		cout << networks[0].neurons.size() - 19 << ' ';
		for(int i = 0; i < 10; i++){
			cout << networks[i].error << ' ';
		}
		cout << endl;
	}

	playHuman(networks[0]);

	return 0;
}
