#ifndef NEURON_HPP_
#define NEURON_HPP_
#include <vector>
#include <utility>

using namespace std;
class neuron
{
public:
	neuron(bool a_IsInput, bool a_IsOutput) :
	 input(a_IsInput),
	 output(a_IsOutput),
	 computed(false),
	 value(0.0),
	 bias(0.0)
	{
		if(input){
			computed = true;
		}
		// Empty
	}

	void print() const
	{
		cout << "Neuron:" << endl;
		for(const auto&link : into){
			cout << "From: " << link.first << " weight " << link.second << endl;
		}
	}

	double getValue() const
	{
		if(!computed){
			cout << "Error, not computed" << endl;
			print();
			exit(0);
		}
		return value;
	}

	void setValue(double a_Value)
	{
		value = a_Value;
	}

	void add(double a_Value)
	{
		value += a_Value;
	}

	void clear()
	{
		if(!input) {
			setValue(0.0);
		}
		computed = false;
	}

	void compute(const vector<neuron> &neurons)
	{
		value = 0;
		for (auto& i : into) {
			value += neurons[i.first].getValue() * i.second;
		}

		value = 2.0 / (1.0 + exp(-1.0 * value));
		computed = true;
	}

	void addInto(int from, double weight)
	{
		for(int i = 0; i < into.size(); i++){
			if(into[i].first == from){
				into[i].second += weight;
				return;
			}
		}
		into.push_back(pair<int,double>(from, weight));
	}

	bool isInput() { return input; }
	bool isOutput() { return output; }

	vector<pair<int, double>> into;
private:
	bool input;
	bool output;
	bool computed;
	double value;
	double bias;
};

#endif
