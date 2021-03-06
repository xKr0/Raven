#include "CNeuralNet.h"
#include "../DataFileNeuron.h"
//#define DISPLAY_TO_CONSOLE "ON"
//#define DISPLAY_DESIRED_VS_OBTAINED  "ON"


//*************************** methods for Neuron **********************
//
//---------------------------------------------------------------------
SNeuron::SNeuron(int NumInputs, int NID): m_iNumInputs(NumInputs+1),
								 m_iID(NID),
                                 m_dActivation(0),
                                 m_dError(0)
											
{
	//we need an additional weight for the bias hence the +1
	for (int i=0; i<NumInputs+1; ++i)
	{
		//set up the weights with an initial random value
		m_vecWeight.push_back(RandomClamped());

		m_vecPrevUpdate.push_back(0);// weight
	}
}




//************************ methods for NeuronLayer **********************

//-----------------------------------------------------------------------
//	ctor creates a layer of neurons of the required size by calling the 
//	SNeuron ctor the rqd number of times
//-----------------------------------------------------------------------
SNeuronLayer::SNeuronLayer(int NumNeurons, 
                           int NumInputsPerNeuron,
						   int IDofLayer):	m_iNumNeurons(NumNeurons), m_iLayerID(IDofLayer)
{
	for (int i=0; i<NumNeurons; ++i)

		m_vecNeurons.push_back(SNeuron(NumInputsPerNeuron, i));
}




//************************ methods forCNeuralNet ************************



//------------------------------- ctor -----------------------------------
//
//------------------------------------------------------------------------
CNeuralNet::CNeuralNet(int NumInputs,
                       int NumOutputs,
                       int HiddenNeurons,
                       double LearningRate,
					   bool softmax		  ):m_iNumInputs(NumInputs),
                                            m_iNumOutputs(NumOutputs),
                                            m_iNumHiddenLayers(NUM_HIDDEN_LAYERS),
											m_bSoftMax(softmax),
                                            m_iNeuronsPerHiddenLyr(HiddenNeurons),
                                            m_dLearningRate(LearningRate),
                                            m_dErrorSum(9999),
                                            m_bTrained(false),
                                            m_iNumEpochs(0)
{
	CreateNet();
	InitializeNetwork();
	//cout << "Initialized networked" << endl;
	//cout << "m_dErrorSum | ERROR_THRESHOLD" << endl;
}


//------------------------------createNet()------------------------------
//
//	this method builds the ANN. The weights are all initially set to 
//	random values -1 < w < 1
//------------------------------------------------------------------------
void CNeuralNet::CreateNet()
{
	int LayerID = 0;
	//create the layers of the network
	if (m_iNumHiddenLayers > 0)
	{
		//create first hidden layer
	  m_vecLayers.push_back(SNeuronLayer(m_iNeuronsPerHiddenLyr, m_iNumInputs, LayerID));
	  LayerID++;
    
    for (int i=0; i<m_iNumHiddenLayers-1; ++i)
    {

			m_vecLayers.push_back(SNeuronLayer(m_iNeuronsPerHiddenLyr,
                                         m_iNeuronsPerHiddenLyr, LayerID));
			LayerID++;
    }

    //create output layer
	  m_vecLayers.push_back(SNeuronLayer(m_iNumOutputs, m_iNeuronsPerHiddenLyr, LayerID));
	}

  else
  {
	  //create output layer
	  m_vecLayers.push_back(SNeuronLayer(m_iNumOutputs, m_iNumInputs, LayerID));
  }
}


//--------------------------- Initialize ---------------------------------
//
//  randomizes all the weights to values btween 0 and 1
//------------------------------------------------------------------------
void CNeuralNet::InitializeNetwork()
{
	ifstream RecordedWeightsIn;
	RecordedWeightsIn.open("RNweightsIn.txt");
	if (RecordedWeightsIn.fail())
		cout << "Failed to open RNweightsIn.txt for reading, weights will be randomnized" << endl;

	double SavedWeights;

	//for each layer
	for (int i=0; i<m_iNumHiddenLayers + 1; ++i)
	{
		//for each neuron
		for (int n=0; n<m_vecLayers[i].m_iNumNeurons; ++n)
		{
			//for each weight
			for (int k=0; k<m_vecLayers[i].m_vecNeurons[n].m_iNumInputs; ++k)
			{
				if (!RecordedWeightsIn.fail())
				{
					RecordedWeightsIn >> SavedWeights;
					m_vecLayers[i].m_vecNeurons[n].m_vecWeight[k] = SavedWeights;
				}
				else
					m_vecLayers[i].m_vecNeurons[n].m_vecWeight[k] = RandomClamped();
			}
		}
	}
	if (!RecordedWeightsIn.fail())
		RecordedWeightsIn.close();

	m_dErrorSum  = 9999;
	m_iNumEpochs = 0;

	return;
}

//-------------------------------Update-----------------------------------
//
//	given an input vector this function calculates the output vector
//
//------------------------------------------------------------------------
vector<double> CNeuralNet::Update(vector<double> inputs)
{
	//Added Jitter
	for (int k = 0; k<inputs.size(); ++k)
	{
		inputs[k] += RandFloat() * MAX_NOISE_TO_ADD;
	}

	//stores the resultant outputs from each layer
	vector<double> outputs;

	int cWeight = 0;
	
	//first check that we have the correct amount of inputs
	if (inputs.size() != m_iNumInputs)
	{
		//just return an empty vector if incorrect.
		return outputs;
	}
	
	//For each layer...
	for (int i=0; i<m_iNumHiddenLayers + 1; ++i)
	{
		
		if ( i > 0 )
		{
			inputs = outputs;
		}

		outputs.clear();
		
		cWeight = 0;

		//for each neuron sum the (inputs * corresponding weights).Throw 
		//the total at our sigmoid function to get the output.
		for (int n=0; n<m_vecLayers[i].m_iNumNeurons; ++n)
		{
			double netinput = 0;

			int	NumInputs = m_vecLayers[i].m_vecNeurons[n].m_iNumInputs;
			
			//for each weight
			for (int k=0; k<NumInputs - 1; ++k)
			{
				//sum the weights x inputs
				netinput += m_vecLayers[i].m_vecNeurons[n].m_vecWeight[k] * 
                    inputs[cWeight++];
			}

			//add in the bias
			netinput += m_vecLayers[i].m_vecNeurons[n].m_vecWeight[NumInputs-1] * 
                  BIAS;
#ifdef DISPLAY_TO_CONSOLE
			cout << "Layer " << m_vecLayers[i].m_iLayerID << " net input at neuron #"
				<< m_vecLayers[i].m_vecNeurons[n].m_iID << ": " << netinput << endl;
#endif // DISPLAY_TO_CONSOLE

			//softmax on output layers
			if (m_bSoftMax && (i == m_iNumHiddenLayers))
			{
				m_vecLayers[i].m_vecNeurons[n].m_dActivation = exp(netinput);

			}
			else
			{
				//The combined activation is first filtered through the sigmoid 
				//function and a record is kept for each neuron 
				m_vecLayers[i].m_vecNeurons[n].m_dActivation =
					Sigmoid(netinput, ACTIVATION_RESPONSE);
			}

#ifdef DISPLAY_TO_CONSOLE
			cout << "Layer " << m_vecLayers[i].m_iLayerID << " activation (output) at neuron #" 
				<< m_vecLayers[i].m_vecNeurons[n].m_iID << ": " << m_vecLayers[i].m_vecNeurons[n].m_dActivation << endl;
#endif // DISPLAY_TO_CONSOLE
			//store the outputs from each layer as we generate them.
	  
		 outputs.push_back(m_vecLayers[i].m_vecNeurons[n].m_dActivation);

			cWeight = 0;
		}
	}

	if (m_bSoftMax)
	{
		double expTot = 0;

		//first calculate the exp for the sum of the outputs
		for (int o = 0; o<outputs.size(); ++o)
		{
			expTot += outputs[o];
		}

		//now adjust each output accordingly
		for (int o = 0; o<outputs.size(); ++o)
		{
			outputs[o] = outputs[o] / expTot;

			m_vecLayers[m_iNumHiddenLayers].m_vecNeurons[o].m_dActivation = outputs[o];
		}
	}

	return outputs;
}

//----------------------------NetworkTrainingEpoch -----------------------
//
//  given a training set this method trains the network using backprop.
//  The training sets comprise of series of input vectors and a series
//  of output vectors.
//  Returns false if there is a problem
//------------------------------------------------------------------------
bool CNeuralNet::NetworkTrainingEpoch(vector<iovector> &SetIn,
                                      vector<iovector> &SetOut)
{
  //create some iterators
  vector<double>::iterator  curWeight;
  vector<SNeuron>::iterator curNrnOut, curNrnHid;

  double WeightUpdate = 0;

  //this will hold the cumulative error value for the training set
  m_dErrorSum = 0;

  //run each input pattern through the network, calculate the errors and update
  //the weights accordingly
  for (int vec=0; vec<SetIn.size(); ++vec)
  {
    //first run this input vector through the network and retrieve the outputs
#ifdef DISPLAY_TO_CONSOLE
	  cout << endl << "***New Cycle***" << endl;
#endif

    vector<double> outputs = Update(SetIn[vec]);

    //return if error has occurred
    if (outputs.size() == 0)
    {
      return false;
    }

    //for each output neuron calculate the error and adjust weights
    //accordingly
    for (int op=0; op<m_iNumOutputs; ++op)
    {
      //first calculate the error value
#ifdef DISPLAY_DESIRED_VS_OBTAINED
		if (/*(vec % 10000 == 0) || */(vec >= SetIn.size() - 3))
			cout << "Desired output: " << SetOut[vec][op] << " | " << "Neural Net output: " << outputs[op] << endl;
#endif

      double err = (SetOut[vec][op] - outputs[op]) * outputs[op]
                   * (1 - outputs[op]);    

#ifdef DISPLAY_DESIRED_VS_OBTAINED
	  if (/*(vec % 10000 == 0) || */(vec >= SetIn.size() - 3))
		 cout << "Calculated error = " << err << endl;
#endif

      //keep a record of the error value
      m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_dError = err;

	  /*softmax*
      //update the SSE. (when this value becomes lower than a
      //preset threshold we know the training is successful)
      m_dErrorSum += (SetOut[vec][op] - outputs[op]) *
                     (SetOut[vec][op] - outputs[op]); 
	  /**/



      curWeight = m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_vecWeight.begin();
      curNrnHid = m_vecLayers[0].m_vecNeurons.begin();

	  int w = 0;

      //for each weight up to but not including the bias
      while(curWeight != m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_vecWeight.end()-1)
      {
		  //calculate weight update
		  WeightUpdate = err * m_dLearningRate * curNrnHid->m_dActivation; //weight

#ifdef DISPLAY_TO_CONSOLE
		  cout << "Layer " << m_vecLayers[OUTPUT_LAYER].m_iLayerID << " neuron #" << m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_iID
			  << " weight #" << w << " " << *curWeight;
#endif

        //calculate the new weight based on the backprop rules
       // *curWeight += err * m_dLearningRate * curNrnHid->m_dActivation;
		  *curWeight += WeightUpdate + m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_vecPrevUpdate[w] * MOMENTUM; // weight

#ifdef DISPLAY_TO_CONSOLE
		  if (WeightUpdate != m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_vecPrevUpdate[w])
			  cout << " adjusted to " << *curWeight << endl;
		  else
			  cout << " unchanged" << endl;
#endif

	   //keep a record of this weight update
		  m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_vecPrevUpdate[w] = WeightUpdate;
		  ++w;

		

        ++curWeight; ++curNrnHid;
      }

      //and the bias for this neuron
      //*curWeight += err * m_dLearningRate * BIAS;     
	  //and the bias for this neuron	//weight
	  WeightUpdate = err * m_dLearningRate * BIAS;

#ifdef DISPLAY_TO_CONSOLE
	  cout << "Layer " << m_vecLayers[OUTPUT_LAYER].m_iLayerID << " neuron #" << m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_iID
		  << " Bias weight of " << *curWeight;
#endif

	  *curWeight += WeightUpdate + m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_vecPrevUpdate[w] * MOMENTUM;

#ifdef DISPLAY_TO_CONSOLE
	  if (WeightUpdate != m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_vecPrevUpdate[w])
		  cout << " adjusted to " << *curWeight << endl;
	  else
		  cout << " unchanged" << endl;
#endif
	  //keep a record of this weight update
	  m_vecLayers[OUTPUT_LAYER].m_vecNeurons[op].m_vecPrevUpdate[w] = WeightUpdate;
	  //weight
    }

	//update the error total. (when this value becomes lower than a
	//preset threshold we know the training is successful)
	double error = 0;

	if (!m_bSoftMax) //Use SSE
	{
		for (int o = 0; o<m_iNumOutputs; ++o)
		{

			error += (SetOut[vec][o] - outputs[o]) *
				(SetOut[vec][o] - outputs[o]);
		}
	}

	else  //use cross-entropy error
	{
		for (int o = 0; o<m_iNumOutputs; ++o)
		{
			error += SetOut[vec][o] * log(outputs[o]);
		}

		error = -error;
	}

	m_dErrorSum += error;

#ifdef DISPLAY_TO_CONSOLE
	cout << "Error sum = " << m_dErrorSum << endl;
#endif


   //**moving backwards to the hidden layer**
    curNrnHid = m_vecLayers[0].m_vecNeurons.begin();

    int n = 0;
    
    //for each neuron in the hidden layer calculate the error signal
    //and then adjust the weights accordingly
    while(curNrnHid != m_vecLayers[0].m_vecNeurons.end())
    {
      double err = 0;

      curNrnOut = m_vecLayers[OUTPUT_LAYER].m_vecNeurons.begin();

      //to calculate the error for this neuron we need to iterate through
      //all the neurons in the output layer it is connected to and sum
      //the error * weights
      while(curNrnOut != m_vecLayers[OUTPUT_LAYER].m_vecNeurons.end())
      {
        err += curNrnOut->m_dError * curNrnOut->m_vecWeight[n];

        ++curNrnOut;
      }

      //now we can calculate the error
      err *= curNrnHid->m_dActivation * (1 - curNrnHid->m_dActivation);     

#ifdef DISPLAY_TO_CONSOLE
	  cout << "Hidden layer neuron #" << curNrnHid->m_iID << " calculated error is " << err << endl;
#endif
      
      //for each weight in this neuron calculate the new weight based
      //on the error signal and the learning rate
	  int w;
      for (w=0; w<m_iNumInputs; ++w)
      {
        //calculate the new weight based on the backprop rules
       // curNrnHid->m_vecWeight[w] += err * m_dLearningRate * SetIn[vec][w];

		  WeightUpdate = err * m_dLearningRate * SetIn[vec][w]; //weight

#ifdef DISPLAY_TO_CONSOLE
		  if (err > 0)
			 cout << "Hidden layer neuron #" << curNrnHid->m_iID << " weight of " << curNrnHid->m_vecWeight[w];
#endif
			 
		  //calculate the new weight based on the backprop rules and adding in momentum
		  curNrnHid->m_vecWeight[w] += WeightUpdate + curNrnHid->m_vecPrevUpdate[w] * MOMENTUM;

#ifdef DISPLAY_TO_CONSOLE
		  if (err > 0)
			cout << " updated to " << curNrnHid->m_vecWeight[w] << endl;
#endif

		  //keep a record of this weight update
		  curNrnHid->m_vecPrevUpdate[w] = WeightUpdate; //weight
      }

      //and the bias
      //curNrnHid->m_vecWeight[m_iNumInputs] += err * m_dLearningRate * BIAS;

	  WeightUpdate = err * m_dLearningRate * BIAS; // weight

#ifdef DISPLAY_TO_CONSOLE
	  if (err > 0)
			cout << "Hidden layer neuron #" << curNrnHid->m_iID << " Bias weight of " << curNrnHid->m_vecWeight[w];
#endif

	  curNrnHid->m_vecWeight[m_iNumInputs] += WeightUpdate + curNrnHid->m_vecPrevUpdate[w] * MOMENTUM;

#ifdef DISPLAY_TO_CONSOLE
	  if (err > 0)
			cout << " updated to " << curNrnHid->m_vecWeight[w] << endl;
#endif

	  //keep a record of this weight update
	  curNrnHid->m_vecPrevUpdate[w] = WeightUpdate; // weight

      ++curNrnHid;
      ++n;
    }

	
	/**
	ofstream DataOutFile;
	string InputFile = "result.txt";
	DataOutFile.open(InputFile.c_str(), ios::app);
	if (DataOutFile.fail())
	{
		cout << "fail to create result.txt" << endl;
	}
	DataOutFile << curNrnHid->m_iID << " " << *curWeight << endl;
	for (int vec = 0; vec < SetIn.size(); ++vec)
	{

		
	}
	DataOutFile.close();
	///////////////////////**/

  }//next input vector
  if (m_iNumEpochs % 100 == 0)
  {
	  WeightsSaveTab.clear();

	  //Save prievous weights of each neurons of each layers
	  for (int i = 0; i < m_iNumHiddenLayers + 1; i++)
	  {
		  curNrnHid = m_vecLayers[i].m_vecNeurons.begin();
		  while (curNrnHid != m_vecLayers[i].m_vecNeurons.end())
		  {
			  WeightsSaveTab.push_back(curNrnHid->m_vecWeight);
			  curNrnHid++;
		  }
	  }
	  DataFileNeuron::Get()->WriteVectorInFile(WeightsSaveTab);
 }
  
  return true;
}


//----------------------------- Train ------------------------------------
//
//  Given some training data in the form of a CData object this function
//  trains the network until the error is within acceptable limits.
// 
//  the HWND is required to give some graphical feedback
//------------------------------------------------------------------------
bool CNeuralNet::Train(CData* data/*, HWND hwnd*/)
{
  vector<vector<double> > SetIn  = data->GetInputSet();
  vector<vector<double> > SetOut = data->GetOutputSet();

   //first make sure the training set is valid
   if ((SetIn.size()     != SetOut.size())  || 
       (SetIn[0].size()  != m_iNumInputs)   ||
       (SetOut[0].size() != m_iNumOutputs))
   {
     //MessageBox(NULL, "Inputs != Outputs", "Error", NULL);
	   cout << "Inputs != Outputs Error" << endl;
    
     return false;
   }
  
   //initialize all the weights to small random values

   //InitializeNetwork();	//modif pour Raven


   //train using backprop until the SSE is below the user defined
   //threshold
   while( m_dErrorSum > ERROR_THRESHOLD )
   {
	   cout << m_dErrorSum << " | " << ERROR_THRESHOLD <<  "  epoch #" << m_iNumEpochs << endl;
     //return false if there are any problems
     if (!NetworkTrainingEpoch(SetIn, SetOut))
     {
       return false;
     }

     ++m_iNumEpochs;

     //call the render routine to display the error sum
    /* InvalidateRect(hwnd, NULL, TRUE);
		 UpdateWindow(hwnd);*/
   }

   m_bTrained = true;
   
   return true;
}


//-------------------------------Sigmoid function-------------------------
//
//------------------------------------------------------------------------
double CNeuralNet::Sigmoid(double netinput, double response)
{
	return ( 1 / ( 1 + exp(-netinput / response)));
}


