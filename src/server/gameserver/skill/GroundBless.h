//////////////////////////////////////////////////////////////////////////////
// Filename    : GroundBless.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_GROUND_BLESS_HANDLER_H__
#define __SKILL_GROUND_BLESS_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class GroundBless;
//////////////////////////////////////////////////////////////////////////////

class GroundBless : public SkillHandler 
{
public:
	GroundBless()  {}
	~GroundBless()  {}
	
public:
    string getSkillHandlerName()  { return "GroundBless"; }
	SkillType_t getSkillType()  { return SKILL_GROUND_BLESS; }

	void execute(Ousters* pOusters,  OustersSkillSlot* pOustersSkillSlot, CEffectID_t CEffectID);
	void execute(Ousters* pOusters, ObjectID_t ObjectID,  OustersSkillSlot* pOustersSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern GroundBless g_GroundBless;

#endif // __SKILL_GROUND_BLESS_HANDLER_H__
