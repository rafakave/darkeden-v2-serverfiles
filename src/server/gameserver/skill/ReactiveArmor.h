//////////////////////////////////////////////////////////////////////////////
// Filename    : ReactiveArmor.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_REACTIVE_ARMOR_HANDLER_H__
#define __SKILL_REACTIVE_ARMOR_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class ReactiveArmor;
//////////////////////////////////////////////////////////////////////////////

class ReactiveArmor : public SkillHandler 
{
public:
	ReactiveArmor()  {}
	~ReactiveArmor()  {}
	
public:
    string getSkillHandlerName()  { return "ReactiveArmor"; }
	SkillType_t getSkillType()  { return SKILL_REACTIVE_ARMOR; }

	void execute(Ousters* pOusters,  OustersSkillSlot* pOustersSkillSlot, CEffectID_t CEffectID);
	void execute(Ousters* pOusters, ObjectID_t ObjectID,  OustersSkillSlot* pOustersSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern ReactiveArmor g_ReactiveArmor;

#endif // __SKILL_REACTIVE_ARMOR_HANDLER_H__
