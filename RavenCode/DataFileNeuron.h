#pragma once
#include <iostream>
#include <fstream>
#include <vector>

/**
	Class to generate the neuron data file,
	It's a singleton so it can be call from everywhere

	For now the data are the following set:
	isShootbale - the target is in sight and can be shoot 
	isNotOutOfViewForTooLong - the target didnt disapear
	isFacingTheTarget - the target is in front of the bot
	isStillThere - the target has not been forgot
	shoot - The bot finally shoot or not
*/
class DataFileNeuron
{
private:
	// the file we write into
	std::ofstream myfile;
	std::ofstream myRNresult;
	// the instance for the singleton
	static DataFileNeuron* instance;
	// class constructor
	DataFileNeuron();

public:
	// method to get the singleton
	static DataFileNeuron* Get();
	// destructor
	~DataFileNeuron();
	// write the vector of value as one line of the file
	void WriteVectorInFile(std::vector<double> vector);
	void WriteVectorInFile(std::vector< std::vector<double> > vector);
};

