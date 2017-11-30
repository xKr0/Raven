#ifndef RAVEN_CACHE_EVALUATOR
#define RAVEN_CACHE_EVALUATOR
#pragma warning (disable:4786)

#include "Goal_Evaluator.h"
#include "../Raven_Bot.h"

class FollowLeader_Evaluator : public Goal_Evaluator
{
private:
	Vector2D m_vCachePos;

	// weapon cache
	Trigger_TeamWeaponCache* triggerCache;

public:

	FollowLeader_Evaluator(double bias) :Goal_Evaluator(bias) {}

	double CalculateDesirability(Raven_Bot* pBot);

	void  SetGoal(Raven_Bot* pEnt);

	void RenderInfo(Vector2D Position, Raven_Bot* pBot);

	void SetCache(Raven_Bot* pBot);
};

#endif