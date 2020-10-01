// Neural_Network.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cassert>

class Neuron;

typedef std::vector<Neuron> Layer;


// ********************* struct Connection ***********************
struct Connection {
	double weight;
	double deltaWeight;
};

// ********************** class Neuron ***************************
class Neuron {
public:
	Neuron(unsigned numOutputs, const unsigned myIndex);

	void setOutputVal(double val) { m_outputVal = val; }
	double getOutputVal(void) { return m_outputVal; }

	void feedForward(const Layer &prevLayer);
private:
	static double randomWeight() { return rand() / double(RAND_MAX); }
	static double transferFunction(double x);
	static double transferFunctionDeriviative(double x);
	double m_outputVal;
	unsigned m_myIndex;
	std::vector<Connection> m_outputWeights;
};

Neuron::Neuron(unsigned numOutputs, const unsigned myIndex) {
	for (unsigned c = 0; c < numOutputs; ++c) {
		m_outputWeights.push_back(Connection());
		m_outputWeights.back().weight = Neuron::randomWeight();
	}

	m_myIndex = myIndex;
}

void Neuron::feedForward(const Layer &prevLayer) {
	double sum = 0.0;

	// Sum the previous layer's outputs(as inputs).
	// Include the bias node from the previous layer.

	for (unsigned n = 0; n < prevLayer.size(); ++n)	{
		sum += prevLayer[n].getOutputVal() * prevLayer[n].m_outputWeights[m_myIndex].weight;
	}

	m_outputVal = Neuron::transferFunction(sum);
}

double Neuron::transferFunction(double x) {
	return tanh(x);
}

double Neuron::transferFunctionDeriviative(double x) {
	return 1.0 - x * x;
}

// ************************ class Net ****************************
class Net {
public:
	Net(const std::vector<unsigned> &topology);
	void feedForward(const std::vector<double> &inputVals);
	void backProp(const std::vector<double> &targetVals) {}
	void getResults(std::vector<double> &inputvals) const {}
private:
	std::vector<Layer> m_layers;
	double m_error;
};

Net::Net(const std::vector<unsigned> &topology) {
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
		m_layers.push_back(Layer());
		std::cout << "Layer Added." << std::endl;
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum) {
			m_layers.back().push_back(Neuron(numOutputs, neuronNum));
			std::cout << "-Neuron Added." << std::endl;
		}
	}
}

void Net::feedForward(const std::vector<double> &inputVals) {
	assert(inputVals.size() == m_layers[0].size() - 1);

	// Assign inputVals to Neurons
	for (unsigned i = 0; i < inputVals.size(); i++) {
		m_layers[0][i].setOutputVal(inputVals[i]);
	}
	// Forward Prop
	for (unsigned layerNum = 0; layerNum < m_layers.size(); ++layerNum) {
		Layer &prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n) {
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

void Net::backProp(const std::vector<double> &targetVals) {
	Layer &outputLayer = m_layers.back();
	m_error = 0.0;

	for (size_t n{0}; n < outputLayer.size() - 1; ++n) {
		double delta = targetVals[n] - outputLayer[n].getOutputVal();
		m_error += delta * delta;
	}
	m_error /= outputLayer.size() - 1;
	m_error = sqrt(m_error);
}

// *********************** MAIN *************************
int main() {
	std::ios::sync_with_stdio(false);
	std::vector<unsigned> topology;
	topology.push_back(65536);
	topology.push_back(256);
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
