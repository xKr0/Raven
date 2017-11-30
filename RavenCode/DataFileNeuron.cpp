#include "DataFileNeuron.h"
#include <iostream>
// init the singleton
DataFileNeuron* DataFileNeuron::instance = nullptr;

DataFileNeuron::DataFileNeuron()
{	
	myfile.open("data.txt", std::fstream::app);
	if (myfile.fail())
		std::cout << "Failed to open data.txt for writing" << std::endl;
	myRNresult.open("RNweights.txt");
	if (myRNresult.fail())
		std::cout << "Failed to open RNweights.txt for writing" << std::endl;
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
	myRNresult.close();
}

void DataFileNeuron::WriteVectorInFile(std::vector<double> vector)
{
	for each (double var in vector)
	{
		myfile << var << "\t";
	}
	
	myfile << "\n";
}

void DataFileNeuron::WriteVectorInFile(std::vector< std::vector<double> > vector)
{
	for (std::vector<double> v : vector)
	{
		for (double var : v)
		{
			myRNresult << var << "\t";
		}
		myRNresult << std::endl;
	}
	myRNresult << "#" << std::endl;
}
