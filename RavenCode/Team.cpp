#include "Team.h"
#include "Raven_Bot.h"
#include "debug/DebugConsole.h"

// nbBot in team Blue
int Team::nbBlue = 0;

// nbBot in team Red
int Team::nbRed = 0;

// points earned for killing a bot
int Team::pointsPerKill = 5;

// score blue
int Team::scoreBlue = 0;

// score red
int Team::scoreRed = 0;

// spot radius
double Team::spotRadius = 15.0f;

// spot pos in team Blue
Vector2D Team::spotBlue(430.0f,380.0f);

// spot pos in team Red
Vector2D Team::spotRed(250.0f, 40.0f);


void Team::createTeam(std::list<Raven_Bot*> mBots)
{
	// list size
	int nbBots = mBots.size();

	// cpt for the loop
	int i = 0;

	// for each bot 
	std::list<Raven_Bot*>::iterator curBot = mBots.begin();
	for (curBot; curBot != mBots.end(); ++curBot)
	{
		if (i == 0) {
			// we change the team value 
			(*curBot)->setTeam(red);
			i++;
		}
		else if (i == 1) {
			// we change the team value 
			(*curBot)->setTeam(blue);
			i--;
		}
	}
	debug_con << "Teams creation is complete : " << Team::nbRed << " Red vs " << Team::nbBlue << " Blue" << "";
}

void Team::destroyTeam(std::list<Raven_Bot*> mBots)
{
	// for each bot 
	std::list<Raven_Bot*>::iterator curBot = mBots.begin();
	for (curBot; curBot != mBots.end(); ++curBot)
	{
		// we change the team value 
		(*curBot)->setTeam(none);
	}

	// display score results in debug console
	if (Team::scoreBlue == Team::scoreRed) {
		debug_con << "Match tie ! Red : " << Team::scoreRed << "pts vs Blue : " << Team::scoreBlue << "pts" << "";
	}
	else if (Team::scoreBlue > Team::scoreRed) {
		debug_con << "Blue wins ! Red : " << Team::scoreRed << "pts vs Blue : " << Team::scoreBlue << "pts" << "";
	}
	else {
		debug_con << "Red wins ! Red : " << Team::scoreRed << "pts vs Blue : " << Team::scoreBlue << "pts" << "";
	}
	
	// reset team counters and scores 
	nbBlue = 0;
	nbRed = 0;
	scoreBlue = 0;
	scoreRed = 0;

	debug_con << "Teams have been destroyed!" << "";
}

void Team::increasedRedScore() {
	Team::scoreRed += Team::pointsPerKill;
	debug_con << "Team Red scores ! Red : " << Team::scoreRed << "pts vs Blue : " << Team::scoreBlue << "pts" << "";
}

void Team::increasedBlueScore() {
	Team::scoreBlue += Team::pointsPerKill;
	debug_con << "Team Blue scores ! Red : " << Team::scoreRed << "pts vs Blue : " << Team::scoreBlue << "pts" << "";
}
