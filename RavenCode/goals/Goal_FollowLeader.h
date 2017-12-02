#pragma once

#include "Goals/Goal.h"
#include "2d/Vector2D.h"
#include "Raven_Goal_Types.h"
#include "../Raven_Bot.h"

class Goal_FollowLeader : public Goal<Raven_Bot>
{
	//the leader the bot is moving to
	Raven_Bot*  m_Leader;

	//the approximate time the bot should take to travel the target location
	double     m_dTimeToReachPos;

	//this records the time this goal was activated
	double     m_dStartTime;

	//returns true if a bot gets stuck
	bool      isStuck()const;

	Vector2D m_pos;

public:

	Goal_FollowLeader(Raven_Bot* pBot, Raven_Bot* leader);

	//the usual suspects
	void Activate();
	int  Process();
	void Terminate();

	void Render();
};
