//////////////////////////////////////////////////////////////////////////////
// Filename    : MiracleShield.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_MIRACLE_SHIELD_HANDLER_H__
#define __SKILL_MIRACLE_SHIELD_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class MiracleShield;
//////////////////////////////////////////////////////////////////////////////

class MiracleShield : public SkillHandler 
{
public:
	MiracleShield()  {}
	~MiracleShield()  {}
	
public:
    string getSkillHandlerName()  { return "MiracleShield"; }
	SkillType_t getSkillType()  { return SKILL_MIRACLE_SHIELD; }

	void execute(Slayer* pSlayer, SkillSlot* pSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern MiracleShield g_MiracleShield;

#endif // __SKILL_MIRACLE_SHIELD_HANDLER_H__
