#ifndef KNIFE_H
#define KNIFE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Knife.h
//
//  Author: Déchaux Quentin
//
//  Desc:   class to implement a knife type projectile
//
//-----------------------------------------------------------------------------
#include "Raven_Projectile.h"

class Raven_Bot;



class Knife : public Raven_Projectile
{
private:

	//tests the trajectory of the shell for an impact
	void TestForImpact();

public:

	Knife(Raven_Bot* shooter, Vector2D target);

	void Render();

	void Update();

};


#endif