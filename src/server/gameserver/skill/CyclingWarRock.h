//////////////////////////////////////////////////////////////////////////////
// Filename    : RingOfFlare.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_CYCLING_WAR_ROCK_HANDLER_H__
#define __SKILL_CYCLING_WAR_ROCK_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class RingOfFlare;
//////////////////////////////////////////////////////////////////////////////

class CyclingWarRock : public SkillHandler 
{
public:
	CyclingWarRock()  {}
	~CyclingWarRock()  {}
	
public:
    string getSkillHandlerName()  { return "CyclingWarRock"; }
	SkillType_t getSkillType()  { return SKILL_CYCLING_WAR_ROCK; }

	void execute(Ousters* pOusters,  OustersSkillSlot* pOustersSkillSlot, CEffectID_t CEffectID);
	void execute(Ousters* pOusters, ObjectID_t TargetObjectID, OustersSkillSlot* pOustersSkillSlot, CEffectID_t CEffectID);
	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern CyclingWarRock g_CyclingWarRock;

#endif // __SKILL_RING_OF_FLARE_HANDLER_H__
