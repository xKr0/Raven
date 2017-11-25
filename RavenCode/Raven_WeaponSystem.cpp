#include "Raven_WeaponSystem.h"
#include "armory/Weapon_RocketLauncher.h"
#include "armory/Weapon_RailGun.h"
#include "armory/Weapon_ShotGun.h"
#include "armory/Weapon_Blaster.h"
#include "armory/Weapon_Knife.h"
#include "Raven_Bot.h"
#include "misc/utils.h"
#include "lua/Raven_Scriptor.h"
#include "Raven_Game.h"
#include "Raven_UserOptions.h"
#include "2D/transformations.h"
#include "fuzzy/FuzzyOperators.h"
#include <vector>



//------------------------- ctor ----------------------------------------------
//-----------------------------------------------------------------------------
Raven_WeaponSystem::Raven_WeaponSystem(Raven_Bot* owner,
                                       double ReactionTime,
                                       double AimAccuracy,
                                       double AimPersistance):m_pOwner(owner),
                                                          m_dReactionTime(ReactionTime),
                                                          m_dAimAccuracy(AimAccuracy),
                                                          m_dAimPersistance(AimPersistance)
{
  Initialize();
  InitializeFuzzyModule();
}

//------------------------- dtor ----------------------------------------------
//-----------------------------------------------------------------------------
Raven_WeaponSystem::~Raven_WeaponSystem()
{
  for (unsigned int w=0; w<m_WeaponMap.size(); ++w)
  {
    delete m_WeaponMap[w];
  }
}

//------------------------------ Initialize -----------------------------------
//
//  initializes the weapons
//-----------------------------------------------------------------------------
void Raven_WeaponSystem::Initialize()
{
  //delete any existing weapons
  WeaponMap::iterator curW;
  for (curW = m_WeaponMap.begin(); curW != m_WeaponMap.end(); ++curW)
  {
    delete curW->second;
  }

  m_WeaponMap.clear();

  //set up the container
  m_pCurrentWeapon = new Blaster(m_pOwner);

  m_WeaponMap[type_blaster]         = m_pCurrentWeapon;
  m_WeaponMap[type_shotgun]         = 0;
  m_WeaponMap[type_rail_gun]        = 0;
  m_WeaponMap[type_rocket_launcher] = 0;
  m_WeaponMap[type_knife]           = 0;
}

//-------------------------------- SelectWeapon -------------------------------
//
//-----------------------------------------------------------------------------
void Raven_WeaponSystem::SelectWeapon()
{ 
  //if a target is present use fuzzy logic to determine the most desirable 
  //weapon.
  if (m_pOwner->GetTargetSys()->isTargetPresent())
  {
    //calculate the distance to the target
    double DistToTarget = Vec2DDistance(m_pOwner->Pos(), m_pOwner->GetTargetSys()->GetTarget()->Pos());

    //for each weapon in the inventory calculate its desirability given the 
    //current situation. The most desirable weapon is selected
    double BestSoFar = MinDouble;

    WeaponMap::const_iterator curWeap;
    for (curWeap=m_WeaponMap.begin(); curWeap != m_WeaponMap.end(); ++curWeap)
    {
      //grab the desirability of this weapon (desirability is based upon
      //distance to target and ammo remaining)
      if (curWeap->second)
      {
        double score = curWeap->second->GetDesirability(DistToTarget);

        //if it is the most desirable so far select it
        if (score > BestSoFar)
        {
          BestSoFar = score;

          //place the weapon in the bot's hand.
          m_pCurrentWeapon = curWeap->second;
        }
      }
    }
  }

  else
  {
    m_pCurrentWeapon = m_WeaponMap[type_blaster];
  }
}

std::list<Raven_Weapon*> Raven_WeaponSystem::GetInventory()
{	
	// create a tmp list
	std::list<Raven_Weapon*> listWeapon = std::list<Raven_Weapon*>();

	// curWeap to iterate
	WeaponMap::const_iterator curWeap;
	// iterate and add them to the list of weapon
	for (curWeap = m_WeaponMap.begin(); curWeap != m_WeaponMap.end(); ++curWeap)
	{
		if (curWeap->second)
		{
			listWeapon.push_back(curWeap->second);
		}
	}

	return listWeapon;
}

//--------------------  AddWeapon ------------------------------------------
//
//  this is called by a weapon affector and will add a weapon of the specified
//  type to the bot's inventory.
//
//  if the bot already has a weapon of this type then only the ammo is added
//-----------------------------------------------------------------------------
void  Raven_WeaponSystem::AddWeapon(unsigned int weapon_type)
{
  //create an instance of this weapon
  Raven_Weapon* w = 0;

  switch(weapon_type)
  {
  case type_rail_gun:

    w = new RailGun(m_pOwner); break;

  case type_shotgun:

    w = new ShotGun(m_pOwner); break;

  case type_rocket_launcher:

    w = new RocketLauncher(m_pOwner); break;

  case type_knife:

	  w = new Knifes(m_pOwner); break;

  }//end switch
  

  //if the bot already holds a weapon of this type, just add its ammo
  Raven_Weapon* present = GetWeaponFromInventory(weapon_type);

  if (present)
  {
    present->IncrementRounds(w->NumRoundsRemaining());

    delete w;
  }
  
  //if not already holding, add to inventory
  else
  {
    m_WeaponMap[weapon_type] = w;
  }
}


//------------------------- GetWeaponFromInventory -------------------------------
//
//  returns a pointer to any matching weapon.
//
//  returns a null pointer if the weapon is not present
//-----------------------------------------------------------------------------
Raven_Weapon* Raven_WeaponSystem::GetWeaponFromInventory(int weapon_type)
{
  return m_WeaponMap[weapon_type];
}

//----------------------- ChangeWeapon ----------------------------------------
void Raven_WeaponSystem::ChangeWeapon(unsigned int type)
{
  Raven_Weapon* w = GetWeaponFromInventory(type);

  if (w) m_pCurrentWeapon = w;
}

//--------------------------- TakeAimAndShoot ---------------------------------
//
//  this method aims the bots current weapon at the target (if there is a
//  target) and, if aimed correctly, fires a round
//-----------------------------------------------------------------------------
void Raven_WeaponSystem::TakeAimAndShoot()const
{

  double isShootbale = m_pOwner->GetTargetSys()->isTargetShootable();
  double isNotOutOfViewForTooLong = m_pOwner->GetTargetSys()->GetTimeTargetHasBeenOutOfView();
  double isFacingTheTarget = false;
  double isStillThere = false;
  double shoot = false;

  //aim the weapon only if the current target is shootable or if it has only
  //very recently gone out of view (this latter condition is to ensure the 
  //weapon is aimed at the target even if it temporarily dodges behind a wall
  //or other cover)
  if ( isShootbale || (isNotOutOfViewForTooLong < m_dAimPersistance))
  {
    //the position the weapon will be aimed at
    Vector2D AimingPos = m_pOwner->GetTargetBot()->Pos();
	isFacingTheTarget = m_pOwner->RotateFacingTowardPosition(AimingPos);
	isStillThere = (m_pOwner->GetTargetSys()->GetTimeTargetHasBeenVisible() >
		m_dReactionTime);
    
    //if the current weapon is not an instant hit type gun the target position
    //must be adjusted to take into account the predicted movement of the 
    //target
    if (GetCurrentWeapon()->GetType() == type_rocket_launcher ||
        GetCurrentWeapon()->GetType() == type_blaster)
    {
      AimingPos = PredictFuturePositionOfTarget();

      //if the weapon is aimed correctly, there is line of sight between the
      //bot and the aiming position and it has been in view for a period longer
      //than the bot's reaction time, shoot the weapon
	  isFacingTheTarget = m_pOwner->RotateFacingTowardPosition(AimingPos);
      if ( isFacingTheTarget && isStillThere && m_pOwner->hasLOSto(AimingPos) )
      {
        AddNoiseToAim(AimingPos);

        GetCurrentWeapon()->ShootAt(AimingPos);
		shoot = true;
      }
    }

    //no need to predict movement, aim directly at target
    else
    {
      //if the weapon is aimed correctly and it has been in view for a period
      //longer than the bot's reaction time, shoot the weapon
      if (isFacingTheTarget && isStillThere)
      {
        AddNoiseToAim(AimingPos);
        
        GetCurrentWeapon()->ShootAt(AimingPos);
		shoot = true;
      }
    }

	
  }
  
  //no target to shoot at so rotate facing to be parallel with the bot's
  //heading direction
  else
  {
    m_pOwner->RotateFacingTowardPosition(m_pOwner->Pos()+ m_pOwner->Heading());
  }

  // we finally add the line to our file
  std::vector<double> line;
  line.push_back(isShootbale);
  line.push_back(isNotOutOfViewForTooLong);
  line.push_back(isFacingTheTarget);
  line.push_back(isStillThere);
  line.push_back(shoot);
  // we write it in the file
  DataFileNeuron::Get()->WriteVectorInFile(line);

}

//---------------------------- AddNoiseToAim ----------------------------------
//
//  adds a random deviation to the firing angle not greater than m_dAimAccuracy 
//  rads
//-----------------------------------------------------------------------------
void Raven_WeaponSystem::AddNoiseToAim(Vector2D& AimingPos)const
{
  Vector2D toPos = AimingPos - m_pOwner->Pos();

  // Time elapsed with opponent visible, distance with him and velocity
  double TimeOpponentStayVisible = m_pOwner->GetTargetSys()->GetTimeTargetHasBeenVisible();
  double DistToOpponent = Vec2DDistance(m_pOwner->Pos(), m_pOwner->GetTargetBot()->Pos());
  Vector2D BotToBot = (m_pOwner->GetTargetBot()->Pos() - m_pOwner->Pos());
  BotToBot.Normalize();
  double BotVelocityDirection = BotToBot.Dot(m_pOwner->Velocity());


  //forgotten because always at 1 (0.9999....)
  /*double BotVelocitySpeed = m_pOwner->Velocity().Length(); */

  // Modify the Aim
  m_pOwner->GetWeaponSys()->m_dAimAccuracy = m_pOwner->GetWeaponSys()->GetAccuracy(TimeOpponentStayVisible, DistToOpponent/*, BotVelocitySpeed*/, BotVelocityDirection);

  Vec2DRotateAroundOrigin(toPos, RandInRange(-m_dAimAccuracy, m_dAimAccuracy));

  AimingPos = toPos + m_pOwner->Pos();
}

////**************************************
////**************************************
////**************************************
////**************************************
////**************************************
////**************************************
////**************************************

//---------------------------- Accuracy -----------------------------------
//
//-------------------------------------------------------------------------
double Raven_WeaponSystem::GetAccuracy(double TimeOpponentStayVisible, double DistToOpponent/*, double BotVelocitySpeed*/, double BotVelocityDirection)
{
	/*if (m_iNumRoundsLeft == 0)
	{
		m_dLastDesirabilityScore = 0;
	}
	else
	{*/

		//fuzzify distance 

		m_FuzzyModule.Fuzzify("TimeStayVisible", TimeOpponentStayVisible);
		m_FuzzyModule.Fuzzify("DistanceToTarget", DistToOpponent);
		//m_FuzzyModule.Fuzzify("VelocitySpeed", BotVelocitySpeed);
		m_FuzzyModule.Fuzzify("VelocityDirection", BotVelocityDirection);

		m_dLastAccuracyScore = m_FuzzyModule.DeFuzzify("Accuracy", FuzzyModule::max_av);

	/*}*/

	return m_dLastAccuracyScore;
}

//----------------------- InitializeFuzzyModule -------------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void Raven_WeaponSystem::InitializeFuzzyModule()
{

	// DECLARATION OF FUZZY VARIABLES

	FuzzyVariable& Accuracy = m_FuzzyModule.CreateFLV("Accuracy");

	// Accuracy between 0 and 0.2
	FzSet& Low_Accuracy = Accuracy.AddRightShoulderSet("Low_Accuracy", 0.1, 0.15, 0.2);
	FzSet& Medium_Accuracy = Accuracy.AddTriangularSet("Medium_Accuracy", 0.05, 0.1, 0.15);
	FzSet& High_Accuracy = Accuracy.AddLeftShoulderSet("High_Accuracy", 0, 0.05, 0.1);

	// To see how bad the aim can be
	/*
	FzSet& Low_Accuracy = Accuracy.AddRightShoulderSet("Low_Accuracy", 0.5, 0.8, 1.2);
	FzSet& Medium_Accuracy = Accuracy.AddTriangularSet("Medium_Accuracy", 0.2, 0.5, 0.8);
	FzSet& High_Accuracy = Accuracy.AddLeftShoulderSet("High_Accuracy", 0, 0.3, 0.5);*/

	FuzzyVariable& DistanceToTarget = m_FuzzyModule.CreateFLV("DistanceToTarget");

	FzSet& Target_Close = DistanceToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
	FzSet& Target_Medium = DistanceToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
	FzSet& Target_Far = DistanceToTarget.AddRightShoulderSet("Target_Far", 150, 300, 1000);


	FuzzyVariable& TimeStayVisible = m_FuzzyModule.CreateFLV("TimeStayVisible");

	FzSet& ShortPeriod = TimeStayVisible.AddLeftShoulderSet("ShortPeriod", 0, 0.5, 1);
	FzSet& MediumPeriod = TimeStayVisible.AddTriangularSet("MediumPeriod", 0.5, 1.25, 2);
	FzSet& LongPeriod = TimeStayVisible.AddRightShoulderSet("LongPeriod", 1.25, 2, 3600); // infiny

	/*FuzzyVariable& VelocitySpeed = m_FuzzyModule.CreateFLV("VelocitySpeed");

	FzSet& LowSpeed = VelocitySpeed.AddLeftShoulderSet("LowSpeed", 0, 25, 150);
	FzSet& NormalSpeed = VelocitySpeed.AddTriangularSet("NormalSpeed", 25, 150, 300);
	FzSet& QuickSpeed = VelocitySpeed.AddRightShoulderSet("QuickSpeed", 150, 300, 1000);*/

	FuzzyVariable& VelocityDirection = m_FuzzyModule.CreateFLV("VelocityDirection");

	FzSet& InFrontMoveForward = VelocityDirection.AddLeftShoulderSet("InFrontMoveForward", 0, 0.2, 0.3);
	FzSet& InFrontMoveBack = VelocityDirection.AddRightShoulderSet("InFrontMoveBack", -0.3, -0.2, 0);
	FzSet& TranslationSideForward = VelocityDirection.AddRightShoulderSet("TranslationSideForward", 0.2, 0.5, 1.2);
	FzSet& TranslationSideBack = VelocityDirection.AddLeftShoulderSet("TranslationSideBack", -1.2, -0.5, -0.2);



	// RULES
		
	// Target_Close
	m_FuzzyModule.AddRule(FzAND(Target_Close, ShortPeriod, InFrontMoveForward), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Close, ShortPeriod, InFrontMoveBack), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Close, ShortPeriod, TranslationSideForward), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Close, ShortPeriod, TranslationSideBack), Low_Accuracy);

	m_FuzzyModule.AddRule(FzAND(Target_Close, MediumPeriod, InFrontMoveForward), High_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Close, MediumPeriod, InFrontMoveBack), High_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Close, MediumPeriod, TranslationSideForward), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Close, MediumPeriod, TranslationSideBack), Medium_Accuracy);

	m_FuzzyModule.AddRule(FzAND(Target_Close, LongPeriod, InFrontMoveForward), High_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Close, LongPeriod, InFrontMoveBack), High_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Close, LongPeriod, TranslationSideForward), High_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Close, LongPeriod, TranslationSideBack), High_Accuracy);

	// Target_Medium
	m_FuzzyModule.AddRule(FzAND(Target_Medium, ShortPeriod, InFrontMoveForward), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, ShortPeriod, InFrontMoveBack), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, ShortPeriod, TranslationSideForward), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, ShortPeriod, TranslationSideBack), Low_Accuracy);

	m_FuzzyModule.AddRule(FzAND(Target_Medium, MediumPeriod, InFrontMoveForward), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, MediumPeriod, InFrontMoveBack), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, MediumPeriod, TranslationSideForward), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, MediumPeriod, TranslationSideBack), Low_Accuracy);

	m_FuzzyModule.AddRule(FzAND(Target_Medium, LongPeriod, InFrontMoveForward), High_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, LongPeriod, InFrontMoveBack), High_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, LongPeriod, TranslationSideForward), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, LongPeriod, TranslationSideBack), Medium_Accuracy);

	//Low_Accuracy	Medium_Accuracy	High_Accuracy
	// Target_Far
	m_FuzzyModule.AddRule(FzAND(Target_Far, ShortPeriod, InFrontMoveForward), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Far, ShortPeriod, InFrontMoveBack), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Far, ShortPeriod, TranslationSideForward), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Far, ShortPeriod, TranslationSideBack), Low_Accuracy);

	m_FuzzyModule.AddRule(FzAND(Target_Far, MediumPeriod, InFrontMoveForward), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Far, MediumPeriod, InFrontMoveBack), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Far, MediumPeriod, TranslationSideForward), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Far, MediumPeriod, TranslationSideBack), Low_Accuracy);

	m_FuzzyModule.AddRule(FzAND(Target_Far, LongPeriod, InFrontMoveForward), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Far, LongPeriod, InFrontMoveBack), Medium_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Far, LongPeriod, TranslationSideForward), Low_Accuracy);
	m_FuzzyModule.AddRule(FzAND(Target_Far, LongPeriod, TranslationSideBack), Low_Accuracy);
}


////**************************************
////**************************************
////**************************************
////**************************************
////**************************************
////**************************************
////**************************************


//-------------------------- PredictFuturePositionOfTarget --------------------
//
//  predicts where the target will be located in the time it takes for a
//  projectile to reach it. This uses a similar logic to the Pursuit steering
//  behavior.
//-----------------------------------------------------------------------------
Vector2D Raven_WeaponSystem::PredictFuturePositionOfTarget()const
{
  double MaxSpeed = GetCurrentWeapon()->GetMaxProjectileSpeed();
  
  //if the target is ahead and facing the agent shoot at its current pos
  Vector2D ToEnemy = m_pOwner->GetTargetBot()->Pos() - m_pOwner->Pos();
 
  //the lookahead time is proportional to the distance between the enemy
  //and the pursuer; and is inversely proportional to the sum of the
  //agent's velocities
  double LookAheadTime = ToEnemy.Length() / 
                        (MaxSpeed + m_pOwner->GetTargetBot()->MaxSpeed());
  
  //return the predicted future position of the enemy
  return m_pOwner->GetTargetBot()->Pos() + 
         m_pOwner->GetTargetBot()->Velocity() * LookAheadTime;
}


//------------------ GetAmmoRemainingForWeapon --------------------------------
//
//  returns the amount of ammo remaining for the specified weapon. Return zero
//  if the weapon is not present
//-----------------------------------------------------------------------------
int Raven_WeaponSystem::GetAmmoRemainingForWeapon(unsigned int weapon_type)
{
  if (m_WeaponMap[weapon_type])
  {
    return m_WeaponMap[weapon_type]->NumRoundsRemaining();
  }

  return 0;
}

//---------------------------- ShootAt ----------------------------------------
//
//  shoots the current weapon at the given position
//-----------------------------------------------------------------------------
void Raven_WeaponSystem::ShootAt(Vector2D pos)const
{
  GetCurrentWeapon()->ShootAt(pos);
}

//-------------------------- RenderCurrentWeapon ------------------------------
//-----------------------------------------------------------------------------
void Raven_WeaponSystem::RenderCurrentWeapon()const
{
  GetCurrentWeapon()->Render();
}

void Raven_WeaponSystem::RenderDesirabilities()const
{
  Vector2D p = m_pOwner->Pos();

  int num = 0;
  
  WeaponMap::const_iterator curWeap;
  for (curWeap=m_WeaponMap.begin(); curWeap != m_WeaponMap.end(); ++curWeap)
  {
    if (curWeap->second) num++;
  }

  int offset = 15 * num;

    for (curWeap=m_WeaponMap.begin(); curWeap != m_WeaponMap.end(); ++curWeap)
    {
      if (curWeap->second)
      {
        double score = curWeap->second->GetLastDesirabilityScore();
        std::string type = GetNameOfType(curWeap->second->GetType());

        gdi->TextAtPos(p.x+10.0, p.y-offset, ttos(score) + " " + type);

        offset+=15;
      }
    }
}
