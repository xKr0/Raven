#include "FollowLeader_Evaluator.h"
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "../Raven_WeaponSystem.h"
#include "../Raven_ObjectEnumerations.h"
#include "misc/cgdi.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"
#include "../Raven_UserOptions.h"



#include "debug/DebugConsole.h"

//------------------ CalculateDesirability ------------------------------------
//
//  returns a value between 0 and 1 that indicates the Rating of a bot (the
//  higher the score, the stronger the bot).
//-----------------------------------------------------------------------------
double FollowLeader_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
	double Desirability = 0.0;

	debug_con << "Get LEADER" << pBot->GetLeader();

	if (!(pBot->getTag() == Team::none) && pBot->GetLeader() != nullptr 
		&& pBot->canWalkTo(pBot->GetLeader()->Pos()) && UserOptions->GetFollowLeader()
		&& Vec2DDistance(pBot->Pos(), pBot->GetLeader()->Pos()) > 20) {
		const double Tweaker = 1.2;

		Desirability = Tweaker *
			Raven_Feature::Health(pBot);

		//ensure the value is in the range 0 to 1
		Clamp(Desirability, 0, 1);
		debug_con << "FOLLOW LEADER" << Desirability;
	}

	return Desirability;
}

//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void FollowLeader_Evaluator::SetGoal(Raven_Bot* pBot)
{
	pBot->GetBrain()->AddGoal_FollowLeader(pBot, pBot->GetLeader());
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void FollowLeader_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
	gdi->TextAtPos(Position, "AT: " + ttos(CalculateDesirability(pBot), 2));
	return;

	//std::string s = ttos(Raven_Feature::DistanceToCache(pBot, triggerCache)) + ", " + ttos(Raven_Feature::TotalWeaponStrength(pBot));
	//gdi->TextAtPos(Position + Vector2D(0, 12), s);
}

