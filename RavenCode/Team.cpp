#include "Team.h"
#include "Raven_Bot.h"

// nbBot in team Blue
int Team::nbBlue = 0;

// nbBot in team Red
int Team::nbRed = 0;

void Team::createTeam(std::list<Raven_Bot*> mBots)
{
	// list size
	int nbBots = mBots.size();

	// cpt for the loop
	int i = 0;

	// for each bot 
	std::list<Raven_Bot*>::iterator curBot = mBots.begin();
	for (curBot; i <= nbBots/2; ++curBot, i++)
	{
		// we change the team value 
		(*curBot)->setTeam(red);
	}

	for (curBot; i <= nbBots-1; ++curBot, i++)
	{
		// we change the team value 
		(*curBot)->setTeam(blue);
	}
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

	// reset team counter 
	nbBlue = 0;
	nbRed = 0;
}
