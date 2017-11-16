#include "Trigger_TeamWeaponCache.h"
#include "misc/Cgdi.h"
#include "Raven_Bot.h"
#include "Debug\DebugConsole.h"
#include "armory\Raven_Weapon.h"


Trigger_TeamWeaponCache::Trigger_TeamWeaponCache(Vector2D pos, int radius, int GraphNodeIndex, Team::teamTag tag):
	Trigger_Respawning<Raven_Bot>(BaseGameEntity::m_iNextValidID)
{
	this->tag = tag;

	SetPos(pos);
	SetBRadius(radius);
	SetGraphNodeIndex(GraphNodeIndex);

	//create this trigger's region of fluence
	// TODO CHECK IF THE Region is fine !!!!!!!
	AddCircularTriggerRegion(Pos(), 10);

	//create the vertex buffer for the rocket shape
	const int NumRocketVerts = 8;
	const Vector2D rip[NumRocketVerts] = { Vector2D(0, 3),
		Vector2D(1, 2),
		Vector2D(1, 0),
		Vector2D(2, -2),
		Vector2D(-2, -2),
		Vector2D(-1, 0),
		Vector2D(-1, 2),
		Vector2D(0, 3) };

	for (int i = 0; i<NumRocketVerts; ++i)
	{
		m_vecRLVB.push_back(rip[i]);
	}
}

void Trigger_TeamWeaponCache::Try(Raven_Bot* pBot)
{
	if (this->isActive() && this->isTouchingTrigger(pBot->Pos(), pBot->BRadius()) && this->tag == pBot->getTag())
	{
		for (int weaponType : weaponsInCache)
		{
			debug_con << "Donne l'arme " << weaponType << " au bot " << pBot->ID() << "";
			pBot->GetWeaponSys()->AddWeapon(weaponType);
		}
	}
}


void Trigger_TeamWeaponCache::Render()
{
	if (isActive())
	{
		if (tag == Team::blue) {
			gdi->BluePen();
			gdi->BlueBrush();
			gdi->Circle(Pos(), 3);
			gdi->ThickBluePen();
		}
		else {
			gdi->RedPen();
			gdi->RedBrush();
			gdi->Circle(Pos(), 3);
			gdi->ThickRedPen();
		}
	}
}

void Trigger_TeamWeaponCache::AddWeapons(std::list<Raven_Weapon*> weaponsToAdd)
{
	for (auto& weapon: weaponsToAdd)
	{
		// not a blaster
		if (weapon->GetType() != 9) {
			debug_con << "Ajout de l'arme au spot de la team" << "";
			weaponsInCache.push_back(weapon->GetType());
		}
		
	}
}
