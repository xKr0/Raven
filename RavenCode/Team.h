#ifndef H_TEAM
#define H_TEAM

#include <list>
#include "2D/Vector2D.h"

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

	// points earned for killing a bot
	static int pointsPerKill;

	// nbBot in team Blue
	static int nbBlue;

	// nbBot in team Red
	static int nbRed;

	// score of blue team
	static int scoreBlue;

	// score of red team
	static int scoreRed;

	// position of spot for blue bullet spawn
	static Vector2D spotBlue;

	// position of spot for red bullet spawn
	static Vector2D spotRed;

	// radius of the spot
	static double spotRadius;

	// Separate the list of bot into two different teams
	static void createTeam(std::list<Raven_Bot*> mBots);

	// Destroy the existing team
	static void destroyTeam(std::list<Raven_Bot*> mBots);

	// Increase the score of red team
	static void increasedRedScore();

	// Increase the score of blue team
	static void increasedBlueScore();
};

#endif // TEAM
