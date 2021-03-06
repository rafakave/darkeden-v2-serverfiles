//////////////////////////////////////////////////////////////////////////////
// Filename    : Paralyze.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_MIST_OF_SOUL1_HANDLER_H__
#define __SKILL_MIST_OF_SOUL1_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class Paralyze;
//////////////////////////////////////////////////////////////////////////////

class MistOfSoul1: public SkillHandler 
{
public:
	MistOfSoul1()  {}
	~MistOfSoul1()  {}
	
public:
    string getSkillHandlerName()  { return "MistOfSoul1"; }
	SkillType_t getSkillType()  { return SKILL_MIST_OF_SOUL1; }

	void execute(Ousters* pOusters, OustersSkillSlot* pSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern MistOfSoul1 g_MistOfSoul1;

#endif // __SKILL_PARALYZE_HANDLER_H__
