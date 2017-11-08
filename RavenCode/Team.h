#ifndef H_TEAM
#define H_TEAM

#include <list>

class Raven_Bot;

class Team
{
public:
	// tag for the different team
	enum teamTag
	{
		blue,
		red,
		none
	};

	// nbBot in team Blue
	static int nbBlue;

	// nbBot in team Red
	static int nbRed;

	// Separate the list of bot into two different teams
	static void createTeam(std::list<Raven_Bot*> mBots);

	// Destroy the existing team
	static void destroyTeam(std::list<Raven_Bot*> mBots);
};

#endif // TEAM
