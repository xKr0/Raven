#include "Goal_GoToCache.h"
#include "../Raven_ObjectEnumerations.h"
#include "../Raven_Bot.h"
#include "../navigation/Raven_PathPlanner.h"

#include "Messaging/Telegram.h"
#include "..\Raven_Messages.h"

#include "Goal_SeekToPosition.h"
#include "Goal_FollowPath.h"

#include "debug/DebugConsole.h"

//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void Goal_GoToCache::Activate()
{
	m_iStatus = active;

	debug_con << "GO_TO_CACHE_ACTIVE ! : " << "";

	//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
	RemoveAllSubgoals();

	// get path in graph to the cache spot on map
	if (m_pOwner->GetPathPlanner()->RequestPathToPosition(m_vCachePos))
	{
		AddSubgoal(new Goal_SeekToPosition(m_pOwner, m_vCachePos));
	}
}

//-------------------------- Process ------------------------------------------
//-----------------------------------------------------------------------------
int Goal_GoToCache::Process()
{
	ActivateIfInactive();

	if (cacheHasBeenEmptied())
	{
		Terminate();
	}

	else
	{
		//process the subgoals
		m_iStatus = ProcessSubgoals();
	}

	return m_iStatus;
}

//---------------------------- CacheHasBeenEmptied ----------------------------
//-----------------------------------------------------------------------------
bool Goal_GoToCache::cacheHasBeenEmptied()const
{
	if (m_pCacheTrigger &&
		m_pCacheTrigger->isEmpty() &&
		m_pOwner->hasLOSto(m_pCacheTrigger->Pos()))
	{
		return true;
	}

	return false;
}
