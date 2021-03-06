//////////////////////////////////////////////////////////////////////////////
// Filename    : BloodySpear.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_BLOODY_SPEAR_HANDLER_H__
#define __SKILL_BLOODY_SPEAR_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class BloodySpear;
//////////////////////////////////////////////////////////////////////////////

class BloodySpear : public SkillHandler 
{
public:
	BloodySpear()  {}
	~BloodySpear()  {}
	
public:
    string getSkillHandlerName()  { return "BloodySpear"; }
	SkillType_t getSkillType()  { return SKILL_BLOODY_SPEAR; }

	void execute(Vampire* pVampire, ObjectID_t ObjectID,  VampireSkillSlot* pVampireSkillSlot, CEffectID_t CEffectID);
	void execute(Monster* pMonster, Creature* pEnemy);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern BloodySpear g_BloodySpear;

#endif // __SKILL_BLOODY_SPEAR_HANDLER_H__
