#include "Goal_FollowLeader.h"
#include "../Raven_Bot.h"
#include "../Raven_SteeringBehaviors.h"
#include "time/CrudeTimer.h"
#include "../navigation/Raven_PathPlanner.h"
#include "misc/cgdi.h"



#include "debug/DebugConsole.h"

Goal_FollowLeader::Goal_FollowLeader(Raven_Bot* pBot,
	Raven_Bot* leader) :

	Goal<Raven_Bot>(pBot,
		goal_follow_leader),
	m_Leader(leader),
	m_dTimeToReachPos(0.0)
{}

void Goal_FollowLeader::Activate()
{
	m_iStatus = active;

	//record the time the bot starts this goal
	m_dStartTime = Clock->GetCurrentTime();

	//This value is used to determine if the bot becomes stuck 
	m_dTimeToReachPos = m_pOwner->CalculateTimeToReachPosition(m_Leader->Pos());

	//factor in a margin of error for any reactive behavior
	const double MarginOfError = 1.0;

	m_dTimeToReachPos += MarginOfError;


	//m_pOwner->GetSteering()->SetTarget(m_vPosition);
	m_pOwner->GetSteering()->OffsetPursuitOn(m_Leader, Vector2D(0, 10));

	//m_pOwner->GetSteering()->SeekOn();
}


//------------------------------ Process --------------------------------------
//-----------------------------------------------------------------------------
int Goal_FollowLeader::Process()
{
	//if status is inactive, call Activate()
	ActivateIfInactive();

	//test to see if the bot has become stuck
	if (isStuck())
	{
		m_iStatus = failed;
	}

	//test to see if the bot has reached the waypoint. If so terminate the goal
	else
	{
		if (m_pOwner->isAtPosition(m_Leader->Pos()))
		{
			m_iStatus = completed;
		}
	}

	return m_iStatus;
}

//--------------------------- isBotStuck --------------------------------------
//
//  returns true if the bot has taken longer than expected to reach the 
//  currently active waypoint
//-----------------------------------------------------------------------------
bool Goal_FollowLeader::isStuck()const
{
	double TimeTaken = Clock->GetCurrentTime() - m_dStartTime;

	if (TimeTaken > m_dTimeToReachPos)
	{
		debug_con << "BOT " << m_pOwner->ID() << " IS STUCK!!" << "";

		return true;
	}

	return false;
}


//---------------------------- Terminate --------------------------------------
//-----------------------------------------------------------------------------
void Goal_FollowLeader::Terminate()
{
	//m_pOwner->GetSteering()->SeekOff();
	//m_pOwner->GetSteering()->ArriveOff();
	m_iStatus = completed;
}

//----------------------------- Render ----------------------------------------
//-----------------------------------------------------------------------------
void Goal_FollowLeader::Render()
{
	if (m_iStatus == active)
	{
		gdi->GreenBrush();
		gdi->BlackPen();
		gdi->Circle(m_Leader->Pos(), 3);
	}

	else if (m_iStatus == inactive)
	{

		gdi->RedBrush();
		gdi->BlackPen();
		gdi->Circle(m_Leader->Pos(), 3);
	}
}


