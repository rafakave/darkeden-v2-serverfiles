//////////////////////////////////////////////////////////////////////////////
// Filename    : HandsOfFire.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_HANDS_OF_FIRE_HANDLER_H__
#define __SKILL_HANDS_OF_FIRE_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class HandsOfFire;
//////////////////////////////////////////////////////////////////////////////

class HandsOfFire : public SkillHandler 
{
public:
	HandsOfFire()  {}
	~HandsOfFire()  {}
	
public:
    string getSkillHandlerName()  { return "HandsOfFire"; }
	SkillType_t getSkillType()  { return SKILL_HANDS_OF_FIRE; }

	void execute(Ousters* pOusters,  OustersSkillSlot* pOustersSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern HandsOfFire g_HandsOfFire;

#endif // __SKILL_HANDS_OF_FIRE_HANDLER_H__
