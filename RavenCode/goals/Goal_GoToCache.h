#ifndef GOAL_GOTOCACHE_H
#define GOAL_GOTOCACHE_H
#pragma warning (disable:4786)

#include "Goals/Goal_Composite.h"
#include "Raven_Goal_Types.h"
#include "../Raven_Bot.h"


class Goal_GoToCache : public Goal_Composite<Raven_Bot>
{
private:
	//the position of the cache the bot wants to reach
	Vector2D m_vCachePos;

	Trigger_TeamWeaponCache* m_pCacheTrigger;

public:

	Goal_GoToCache(Raven_Bot* pBot,
		Vector2D   pos,
		Trigger_TeamWeaponCache* trigger) :

		Goal_Composite<Raven_Bot>(	pBot,
									goal_go_to_cache),
		m_vCachePos(pos),
		m_pCacheTrigger(trigger)
	{}

	void Activate();

	int  Process();

	void Terminate() { m_iStatus = completed; }

	//this goal is able to accept messages
	bool HandleMessage(const Telegram& msg);

	bool cacheHasBeenEmptied()const;

};
#endif

