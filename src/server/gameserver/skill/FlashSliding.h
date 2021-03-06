//////////////////////////////////////////////////////////////////////////////
// Filename    : FlashSliding.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_FLASH_SLIDING_HANDLER_H__
#define __SKILL_FLASH_SLIDING_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class FlashSliding;
//////////////////////////////////////////////////////////////////////////////

class FlashSliding : public SkillHandler 
{
public:
	FlashSliding()  {}
	~FlashSliding()  {}
	
public:
    string getSkillHandlerName()  { return "FlashSliding"; }
	SkillType_t getSkillType()  { return SKILL_FLASH_SLIDING; }

	void execute(Slayer* pSlayer, ObjectID_t TargetObjectID, SkillSlot* pSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern FlashSliding g_FlashSliding;

#endif // __SKILL_FLASH_SLIDING_HANDLER_H__
