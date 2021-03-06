//////////////////////////////////////////////////////////////////////////////
// Filename    : BlitzSliding.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_BLITZ_SLIDING_HANDLER_H__
#define __SKILL_BLITZ_SLIDING_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class BlitzSliding;
//////////////////////////////////////////////////////////////////////////////

class BlitzSliding : public SkillHandler 
{
public:
	BlitzSliding()  {}
	~BlitzSliding()  {}
	
public:
    string getSkillHandlerName()  { return "BlitzSliding"; }
	SkillType_t getSkillType()  { return SKILL_BLITZ_SLIDING; }

	void execute(Slayer* pSlayer, ObjectID_t TargetObjectID, SkillSlot* pSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern BlitzSliding g_BlitzSliding;

#endif // __SKILL_BLITZ_SLIDING_HANDLER_H__
