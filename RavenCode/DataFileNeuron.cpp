#include "DataFileNeuron.h"

// init the singleton
DataFileNeuron* DataFileNeuron::instance = nullptr;

DataFileNeuron::DataFileNeuron()
{	
	myfile.open("data.txt");
}

DataFileNeuron* DataFileNeuron::Get()
{
	if (!instance) {
		instance = new DataFileNeuron();
	}

	return instance;
}

DataFileNeuron::~DataFileNeuron()
{
	myfile.close();
}

void DataFileNeuron::WriteVectorInFile(std::vector<double> vector)
{
	for each (double var in vector)
	{
		myfile << var << "\t";
	}

	myfile << "\n";
}
