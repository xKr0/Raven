#pragma once
#include "Team.h"
#include "Triggers/Trigger_Respawning.h"
#include "Raven_WeaponSystem.h"

class Raven_Bot;

class Trigger_TeamWeaponCache : public Trigger_Respawning<Raven_Bot>
{
private:

	//vrtex buffers for rocket shape
	std::vector<Vector2D>         m_vecRLVB;
	std::vector<Vector2D>         m_vecRLVBTrans;

	// team of the cache
	Team::teamTag tag;

	// list of the weapon type in the cache
	std::list<int> weaponsInCache;

public:

	//this type of trigger is created when reading a map file
	Trigger_TeamWeaponCache(Vector2D pos, int radius, int GraphNodeIndex, Team::teamTag tag);

	//if triggered, this trigger will call the PickupWeapon method of the
	//bot. PickupWeapon will instantiate a weapon of the appropriate type.
	void Try(Raven_Bot* pBot);

	//draws a symbol representing the weapon type at the trigger's location
	void Render();

	// add the weapons into the cache
	void AddWeapons(std::list<Raven_Weapon*> weaponsToAdd);

	void SetCacheInactive() { 
		SetInactive(); 
		SetToBeRemovedFromGame();
	};

	void SetCacheActive() { SetActive(); };
};
