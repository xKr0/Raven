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
		//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
	RemoveAllSubgoals();

	// get path in graph to the cache spot on map
	m_pOwner->GetPathPlanner()->RequestPathToPosition(m_vCachePos);

	AddSubgoal(new Goal_SeekToPosition(m_pOwner, m_vCachePos));
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


//---------------------------- HandleMessage ----------------------------------
//-----------------------------------------------------------------------------
bool Goal_GoToCache::HandleMessage(const Telegram& msg)
{
	//first, pass the message down the goal hierarchy
	bool bHandled = ForwardMessageToFrontMostSubgoal(msg);

	//if the msg was not handled, test to see if this goal can handle it
	if (bHandled == false)
	{
		switch (msg.Msg)
		{
		case Msg_PathReady:
			//clear any existing goals
			RemoveAllSubgoals();

			AddSubgoal(new Goal_FollowPath(m_pOwner,
				m_pOwner->GetPathPlanner()->GetPath()));

			return true; //msg handled


		case Msg_NoPathAvailable:
			m_iStatus = failed;

			return true; //msg handled

		default: return false;
		}
	}

	//handled by subgoals
	return true;
}