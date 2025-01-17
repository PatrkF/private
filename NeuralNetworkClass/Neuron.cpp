#include "Neuron.h"

Neuron::Neuron()
{
}

Neuron::Neuron(double* in)
{
	this->setInputOutput(in);
}

Neuron::Neuron(std::vector<Neuron*> synapseIn, SummationEnum typSum, ActivationEnum typActiv)
{
	this->setSynapse(synapseIn);
	this->setSumActiv(typSum, typActiv);
}

Neuron::Neuron(std::vector<Neuron*> synapseIn, double* out, SummationEnum typSum, ActivationEnum typActiv)
{
	this->setSynapse(synapseIn);
	this->setInputOutput(out);
	this->setSumActiv(typSum, typActiv);
}

Neuron::~Neuron()
{
}

void Neuron::process()
{
	if (_dendrites.empty())
	{
		this->axon = *inOutValue;
	}
	else {
		summationFunctions();
		activationFunction();

		if (inOutValue != nullptr)
		{
			*inOutValue = this->getAxon();
		}
	}

}

void Neuron::updateWeightBias()
{
	
}

void Neuron::activationFunction()
{
	switch (this->typActiv)
	{
	case ActivationEnum::Tanh:
		this->axon = tanh(this->soma);
		break;
	case ActivationEnum::Relu:
		if (this->soma > 0)
		{
		this->axon = this->soma;
		}
		else {
			this->axon = 0.0;
		}
		break;
	case ActivationEnum::Sigm:
	default:
		this->axon = (1 / (1 + exp(-this->soma)));
		break;
	}
}

void Neuron::summationFunctions()
{
	this->soma = 0.0;
	this->soma = this->bias;

	switch (this->typSum)
	{
	case SummationEnum::Min:
		
		for (size_t i = 0, tt = _dendrites.size(); i < tt; i++)
		{
			double min = this->inputsDendrites[i]->getAxon() * *this->_dendrites[i];
			if (min < this->soma)
			{
				this->soma = min;
			}
		}
		break;
	case SummationEnum::Max:
		for (size_t i = 0, tt = _dendrites.size(); i < tt; i++)
		{
			double max = this->inputsDendrites[i]->getAxon() * *this->_dendrites[i];
			if (this->soma<max)
			{
				this->soma = max;
			}
		}
		break;
	case SummationEnum::Average:
		for (size_t i = 0, tt = _dendrites.size(); i < tt; i++)
		{
			this->soma += this->inputsDendrites[i]->getAxon() * *this->_dendrites[i];
		}
		this->soma /= this->_dendrites.size();
		break;
	case SummationEnum::Suma:
	default:
		for (size_t i = 0, tt = _dendrites.size(); i < tt; i++)
		{
			this->soma += this->inputsDendrites[i]->getAxon() * *this->_dendrites[i];
		}
		break;
	}
}

void Neuron::derivativesCalculation() //<?> nie tu <?>
{
	for (size_t i = 0, tt = this->_dendrites.size(); i < tt; i++)
	{
		*this->_dendrites[i]=(*this->_dendrites[i] - this->inputsDendrites[i]->getAxon() * this->derivativesError);
	}
	this->setBias(this->getBias() - this->derivativesError);

	this->derivativesError = 0.0;
}

void Neuron::setSynapse(std::vector<Neuron*> synapseIn)
{
	for (size_t i = 0, tt = synapseIn.size(); i < tt; i++) {
		this->_dendrites.push_back(new double(1 - (double)rand() / RAND_MAX));
		this->inputsDendrites.push_back(synapseIn[i]);
	}

}

void Neuron::setBias(double val)
{
	this->bias = val;
}

void Neuron::setInputOutput(double* val)
{
		this->inOutValue = val;
}


void Neuron::setSumActiv(SummationEnum _typSum, ActivationEnum _typActiv)
{
	this->typSum = _typSum;
	this->typActiv = _typActiv;
}

void Neuron::setDerivatives(double cost)
{
	this->derivativesError += cost;
	
}


double Neuron::getAxon()
{
	return this->axon;
}

double Neuron::getDerivativeAxon()
{
	switch (this->typActiv)
	{
	case ActivationEnum::Tanh:
		return (1.0 - (this->getAxon() * this->getAxon()));
		break;
	case ActivationEnum::Relu:
		if (this->soma > 0)
		{
			return 1.0;
		}
		else {
			return 0.0;
		}
		break;
	case ActivationEnum::Sigm:
	default:
		return this->getAxon() * (1 - this->getAxon());
		break;
	}
}

double Neuron::getBias()
{
	return this->bias;
}

std::vector<double*> Neuron::getWeightVec()
{
	return this->_dendrites;
}
