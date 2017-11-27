#include <windows.h>   
#include <stdlib.h>
#include <time.h>

#include "CNeuralNet.h"
#include "defines.h"
#include "CController.h"
#include "CData.h"
#include "resource.h"

//turn off annoying STL warnings
#pragma warning (disable : 4089)


int main() 
{
	cout << "Creating CControler instance..." << endl;
	CController Test;
	cout << "Press ENTER to start";
	cin.ignore();
	Test.TrainNetwork();


	return 0;
}