#ifndef DEFINES_H
#define DEFINES_H

//-----------------------------------------------
//  used in main
//-----------------------------------------------

#define WINDOW_WIDTH        400
#define WINDOW_HEIGHT       400

//define a custom msg we can use to send to the windproc
//to train the network
#define UM_TRAIN (WM_USER + 0)


//-----------------------------------------------
//  used in CData/CMouse
//-----------------------------------------------

//total number of built in patterns
#define NUM_PATTERNS        0 //training commence de rien

#define NUM_OUTPUTS			1

//how many entries each pattern contains
#define NUM_ENTRIES         4	//la 5eme est un output

//output has to be above this value for the program
//to agree on a pattern. Below this value and it
//will try to guess the pattern
#define MATCH_TOLERANCE     0.96



//-----------------------------------------------
//  used in CNeuralNet
//-----------------------------------------------
#define ACTIVATION_RESPONSE 1.0
#define BIAS                -1

//the learning rate for the backprop.
#define LEARNING_RATE       0.2

//when the total error is below this value the 
//backprop stops training
#define ERROR_THRESHOLD     0.003

#define NUM_HIDDEN_LAYERS	1

#define OUTPUT_LAYER NUM_HIDDEN_LAYERS

#define NUM_HIDDEN_NEURONS  6

#define MOMENTUM            0.8	//weight

//how much the input data is perturbed 
#define MAX_NOISE_TO_ADD    0.8

//use softmax or not
#define SOFTMAX				0






#endif