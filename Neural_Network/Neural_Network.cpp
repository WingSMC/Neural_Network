// Neural_Network.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>

class Neuron;

typedef std::vector<Neuron> Layer;

struct Connection {
	double weight;
	double deltaWeight;
};

// ********************** class Neuron ***************************
class Neuron {
public:
	Neuron(unsigned numOutputs) {
		for (unsigned c = 0; c < numOutputs; ++c) {
			m_outputWeights.push_back(Connection());
			m_outputWeights.back().weight = randomWeight();
		}
	}

private:
	static double randomWeight() { return rand() / double(RAND_MAX); }
	double m_outputVal;
	std::vector<Connection> m_outputWeights;
};


// ************************ class Net ****************************
class Net {
public:
	Net(const std::vector<unsigned> &topology) {
		unsigned numLayers = topology.size();
		for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
			m_layers.push_back(Layer());
			std::cout << "Layer Added." << std::endl;
			unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];
			for (unsigned neuronNum = 0; neuronNum < topology[layerNum]; ++neuronNum) {
				m_layers.back().push_back(Neuron(numOutputs));
				std::cout << "-Neuron Added." << std::endl;
			}
			m_layers.back().push_back(Neuron(numOutputs));
			std::cout << "--Bias Neuron Added." << std::endl;
		}
	}
	void feedForward(const std::vector<double> &inputVals) {
		assert(inputVals.size() == m_layers[0].size() - 1);

		// Assign inputVals to Neurons
		for (unsigned i = 0; i < inputVals.size(); i++) {
			m_layers[0][i].setOutputVal(inputVals[i]);
		}
		// Forward Prop
		for (unsigned layerNum = 0; layerNum < m_layers.size(); ++layerNum) {
			Layer &prevLayer = m_layers[layerNum - 1];
			for (unsigned n = 0; i < m_layers[i].size() - 1; i++) {
				m_layers[layerNum][n].feedForward(prevLayer);
			}
		}
	}

	void backProp(const std::vector<double> &targetVals) {}
	void getResults(std::vector<double> &inputvals) const {}
private:
	std::vector<Layer> m_layers;
};


// *********************** MAIN *************************
int main() {
	std::ios::sync_with_stdio(false);
	std::vector<unsigned> topology;
	topology.push_back(3);
	topology.push_back(2);
	topology.push_back(1);
	Net myNet(topology);

	std::vector<double> inputVals;
	myNet.feedForward(inputVals);

	std::vector<double> targetVals;
	myNet.backProp(targetVals);

	std::vector<double> resultVals;
	myNet.getResults(resultVals);

	std::cin.get();

	return 0;
}
