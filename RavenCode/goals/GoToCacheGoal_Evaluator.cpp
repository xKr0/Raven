#include "GoToCacheGoal_Evaluator.h"
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "../Raven_WeaponSystem.h"
#include "../Raven_ObjectEnumerations.h"
#include "misc/cgdi.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"
#include "../Triggers/Trigger_TeamWeaponCache.h"


#include "debug/DebugConsole.h"

//------------------ CalculateDesirability ------------------------------------
//
//  returns a value between 0 and 1 that indicates the Rating of a bot (the
//  higher the score, the stronger the bot).
//-----------------------------------------------------------------------------
double GoToCacheGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
	double Desirability = 0.0;
	SetCache(pBot);

	if (!triggerCache != NULL && !triggerCache->isEmpty()){
	//only do the calculation if there is a cache

		const double Tweaker = 1.0;

		Desirability = Tweaker *
			Raven_Feature::DistanceToCache(pBot, triggerCache) *
			Raven_Feature::TotalWeaponStrength(pBot);
	}

	return Desirability;
}

//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void GoToCacheGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
	pBot->GetBrain()->AddGoal_GoToCache(m_vCachePos, triggerCache);
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void GoToCacheGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
	gdi->TextAtPos(Position, "AT: " + ttos(CalculateDesirability(pBot), 2));
	return;

	std::string s = ttos(Raven_Feature::DistanceToCache(pBot, triggerCache)) + ", " + ttos(Raven_Feature::TotalWeaponStrength(pBot));
	gdi->TextAtPos(Position + Vector2D(0, 12), s);
}

void GoToCacheGoal_Evaluator::SetCache(Raven_Bot* pBot) {
	if (pBot->getTag() == Team::red) {
		m_vCachePos == Team::spotRed;
		triggerCache = Team::teamRed;
	}
	else if (pBot->getTag() == Team::red) {
		m_vCachePos == Team::spotBlue;
		triggerCache = Team::teamBlue;
	}
	else {
		m_vCachePos == pBot->Pos();
		triggerCache = NULL;
	}
}
