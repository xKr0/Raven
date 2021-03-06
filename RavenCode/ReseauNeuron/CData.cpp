#include "CData.h"



//------------------------------------------------------------------------
//
//  constant training data
//------------------------------------------------------------------------
/**
const double InputVectors[NUM_PATTERNS][NUM_VECTORS*2]=
{
	
  1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0,                                 //right

  -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0,                     //left

  0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1,                                 //down

  0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1,                     //up

  1,0, 1,0, 1,0, 0,1, 0,1, 0,1, -1,0, -1,0, -1,0, 0,-1, 0,-1, 0,-1,                           //clockwise square

  -1,0, -1,0, -1,0, 0,1, 0,1, 0,1, 1,0, 1,0, 1,0, 0,-1, 0,-1, 0,-1,                           //anticlockwise square

  1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, -0.45,0.9, -0.9, 0.45, -0.9,0.45,              //Right Arrow 
  
  -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, 0.45,0.9, 0.9, 0.45, 0.9,0.45,        //Left Arrow

  -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7,             //south west
  -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7,

  0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7,            //south east
  0.7,0.7, 0.7,0.7, 0.7,0.7,    
  
  1,0, 1,0, 1,0, 1,0, -0.72,0.69,-0.7,0.72,0.59,0.81, 1,0, 1,0, 1,0, 1,0, 1,0                 //zorro
}; 
/**

const char* Names[NUM_PATTERNS]=
{
  "Right",
  "Left",
  "Down",
  "Up",
  "Clockwise Square",
  "Anti-Clockwise Square",
  "Right Arrow",
  "Left Arrow",
  "South West",
  "South East",
  "Zorro"

};
/**/


//--------------------------------- Init ---------------------------------
//
//  Fetch entries from data.txt and initialize Input/Output vectors.
//
//	Chaques lignes contiennent 4 input + l'output obtenu (avait tirer ou non).
//	Les 4 input sont mis dans un vector, puis push_back dans SetIn. 
//	L'output est mis dans un vector, puis push_back dans SetOut.
//
//	Le réseau va prendre les inputs, produire un output, puis comparer
//	cette output avec celui dans SetOut (l'output qu'il aurais du obtenir) et
//	procede à l'ajustement des poids du réseau.
//	
//	Yadayada calcul d'erreur, continue d'ajuster les poids jusqu'à ce que
//	l'erreur soit dessendu à 0.003 (Il est tard)
//------------------------------------------------------------------------
void CData::Init()
{
	string InputFile = "dataIn.txt";
	ifstream TrainingData;
	TrainingData.open(InputFile.c_str());
	if (TrainingData.fail()) 
	{
		cout << "fail to open dataIn.txt" << endl; 
	}
	else
	{
		m_SetIn.clear(); //les 4 premieres entré, 
		m_SetOut.clear();
		vector<double> DataIn;
		vector<double> DataOut;
		double Entry;
		while (!TrainingData.eof())
		{
			DataIn.clear();
			DataOut.clear();
			for (int i = 0; i < NUM_ENTRIES; i++)
			{
				TrainingData >> Entry;
				DataIn.push_back(Entry);
			}
			TrainingData >> Entry;
			DataOut.push_back(Entry); //l'output Tirer ou Pas Tirer
			
			m_SetIn.push_back(DataIn);
			m_SetOut.push_back(DataOut);
		}
		
	}
 
}

// --------------------------- CreateTrainingSetFromData -----------------
//
//  this function creates a training set from the data defined as constants
//  in CData.h. 
//------------------------------------------------------------------------
void CData::CreateTrainingSetFromData()
{
  //empty the vectors
  m_SetIn.clear();
  m_SetOut.clear();
  
  //add each pattern
  for (int ptn=0; ptn<m_iNumPatterns; ++ptn)
  {    
    //add the data to the training set
    m_SetIn.push_back(m_vecPatterns[ptn]);

    //create the output vector for this pattern. First fill a 
    //std::vector with zeros
    vector<double> outputs(m_iNumPatterns, 0);

    //set the relevant output to 1
    outputs[ptn] = 1;

    //add it to the output set
    m_SetOut.push_back(outputs);
  }
}

//------------------------- PatternName ----------------------------------
//
//  returns the pattern name at the given value
//------------------------------------------------------------------------
string CData::PatternName(int val)
{
  if (m_vecNames.size() > 0)
  {
    return m_vecNames[val];
  }

  else
  {
    return "";
  }
}

//------------------------- AddData -------------------------------------
//
//  adds a new pattern to data
//-----------------------------------------------------------------------
bool CData::AddData(vector<double> &data, string NewName)
{
  //check that the size is correct
  if (data.size() != m_iVectorSize*2)
  {
    MessageBox(NULL, "Incorrect Data Size", "Error", MB_OK);

    return false;
  }

  //add the name
  m_vecNames.push_back(NewName);

  //add the data
  m_vecPatterns.push_back(data);

  //keep a track of number of patterns
  ++m_iNumPatterns;
  
  //create the new training set
  CreateTrainingSetFromData();

  return true; 
}

