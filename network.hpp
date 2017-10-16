#ifndef NETWORK_HPP_
#define NETWORK_HPP_

#include <list>
#include <vector>
#include <algorithm>
#include "neuron.hpp"
#include <assert.h>
#include <stdlib.h>

using namespace std;

const double perturbChance = 0.9;
const double stepSize = 0.05;
const double mutateConnections = 0.4;
const double linkMutationChance = 2.0;
//const double crossoverChance = 0.75;
const double nodeMutationChance = 0.04;
//const double biasMutationChance = 0.4;
//const double disableMutationChance = 0.4;
//const double enableMutationChance = 0.2;

class network
{
public:
	network(int numInputNeuron, int numOutputNeuron)
	{
		neurons.clear();
		computationOrder.clear();

		for(int i = 0; i < numInputNeuron; i++){
			neuron input(true, false);
			addNeuron(input);
		}

		for(int i = 0; i < numOutputNeuron; i++){
			neuron output(false, true);
			addNeuron(output);
		}
	}

	network()
	{

	}

	bool operator<(const network& net)
	{
		return error < net.error;
	}

	void compute(const vector<double> &inputs, vector<double> &outputs)
	{
		assert(inputs.size() == inputNeurons.size());
		outputs.clear();

		for (auto& i : computationOrder) {
			neurons[i].clear();
		}

		for (int i = 0; i < inputs.size(); i++) {
			neurons[inputNeurons[i]].setValue(inputs[i]);
		}

		for (auto& i : computationOrder) {
			neurons[i].compute(neurons);
		}

		for (int i = 0; i < outputNeurons.size(); i++) {
			neurons[outputNeurons[i]].compute(neurons);
			outputs.push_back(neurons[outputNeurons[i]].getValue());
		}
	}

	void addLink(int from, int to, double weight)
	{
		neurons[to].addInto(from, weight);
	}

	void mutate()
	{
		if(randUni() < mutateConnections){
			pointMutate();
		}

		double linkMutateRem = linkMutationChance;
		while(linkMutateRem > 0.0){
			if(randUni() < linkMutateRem){
				linkMutate();
			}

			linkMutateRem -= 1.0;
		}

		double nodeMutateRem = nodeMutationChance;
		while(nodeMutateRem > 0.0){
			if(randUni() < nodeMutateRem){
				nodeMutate();
			}

			nodeMutateRem -= 1.0;
		}
	}

	void addError(double a_Error)
	{
		error += a_Error;
	}
//private:
	void pointMutate()
	{
		for(auto& neutron : neurons){
			for(auto& into : neutron.into){
				if(randUni() < perturbChance){
					into.second = into.second + randUni() * stepSize * 2 - stepSize;
				}
				else {
					into.second = randUni() * stepSize;
				}
			}
		}
	}

	void linkMutate()
	{
		int from = rand() % neurons.size();
		int to = rand() % neurons.size();
		double weight = randUni() * 2 - 1;

		if(from == to) {
			return;
		}

		if(neurons[from].isInput() && neurons[to].isInput()){
			return;
		}

		if(neurons[from].isOutput() && neurons[to].isOutput()){
			return;
		}

		if(neurons[to].isInput()){
			int temp = from;
			from = to;
			to = temp;
		}

		if(neurons[from].isOutput()){
			int temp = from;
			from = to;
			to = temp;
		}

		if(neurons[from].isInput() || neurons[to].isOutput()){
			addLink(from, to, weight);
			return;
		}

		std::vector<int>::iterator itFrom = find(computationOrder.begin(), computationOrder.end(), from);
		std::vector<int>::iterator itTo = find(computationOrder.begin(), computationOrder.end(), to);

		if(itFrom > itTo){
			int temp = from;
			from = to;
			to = temp;
		}

		addLink(from, to, weight);
	}

	void nodeMutate()
	{
		int from = rand() % neurons.size();
		int to = rand() % neurons.size();

		if(from == to) {
			return;
		}

		if(neurons[from].isInput() && neurons[to].isInput()){
			return;
		}

		if(neurons[from].isOutput() && neurons[to].isOutput()){
			return;
		}

		if(neurons[to].isInput()){
			int temp = from;
			from = to;
			to = temp;
		}

		if(neurons[from].isOutput()){
			int temp = from;
			from = to;
			to = temp;
		}

		std::vector<int>::iterator itFrom = find(computationOrder.begin(), computationOrder.end(), from);
		std::vector<int>::iterator itTo = find(computationOrder.begin(), computationOrder.end(), to);

		if(itFrom > itTo){
			int temp = from;
			from = to;
			to = temp;
			itTo = itFrom;
		}

		computationOrder.insert(itTo, neurons.size());
		neurons.push_back(neuron(false, false));
		double weight = stepSize * (randUni() * 2 - 1);
		addLink(from, neurons.size() - 1, weight);
		weight = stepSize * (randUni() * 2 - 1);
		addLink(neurons.size() - 1, to, weight);
	}

	void addNeuron(neuron &a_Neuron)
	{
		if(a_Neuron.isInput()){
			inputNeurons.push_back(neurons.size());
		}

		if(a_Neuron.isOutput()){
			outputNeurons.push_back(neurons.size());
		}

		neurons.push_back(a_Neuron);
	}

	void printNetwork()
	{
		for(const auto& order: computationOrder){
			cout << "Num: " << order << endl;
			neurons[order].print();
		}

		for(const auto& outputs: outputNeurons){
			cout << "Num: " << outputs << endl;
			neurons[outputs].print();
		}
	}

	double randUni() { return (double) rand() / RAND_MAX; }

	vector<neuron> neurons;
	vector<int> computationOrder;
	vector<int> inputNeurons;
	vector<int> outputNeurons;
	double error;
};

#endif
