#ifndef KNIFES_H
#define KNIFES_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Weapon_Knife.h
//
//  Author:Déchaux Quentin
//
//  Desc:   
//-----------------------------------------------------------------------------
#include "Raven_Weapon.h"


class  Raven_Bot;



class Knifes : public Raven_Weapon
{
private:

  void  InitializeFuzzyModule();
  
public:

  Knifes(Raven_Bot*   owner);


  void  Render();

  void  ShootAt(Vector2D pos);

  double GetDesirability(double DistToTarget);
};
#endif