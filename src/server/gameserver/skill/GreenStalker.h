//////////////////////////////////////////////////////////////////////////////
// Filename    : GreenStalker.h 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_GREEN_STALKER_HANDLER_H__
#define __SKILL_GREEN_STALKER_HANDLER_H__

#include "SkillHandler.h"

#define PER_DAMAGE_GREEN_STALKER	10

//////////////////////////////////////////////////////////////////////////////
// class GreenStalker;
//////////////////////////////////////////////////////////////////////////////

class GreenStalker: public SkillHandler 
{
public:
	GreenStalker()  {}
	~GreenStalker()  {}
	
public:
    string getSkillHandlerName()  { return "GreenStalker"; }
	SkillType_t getSkillType()  { return SKILL_GREEN_STALKER; }

	void execute(Vampire* pVampire, ObjectID_t targetObject, VampireSkillSlot* pVampireSkillSlot, CEffectID_t CEffectID);
	void execute(Monster* pMonster, Creature* pEnemy);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern GreenStalker g_GreenStalker;

#endif // __SKILL_GREEN_STALKER_HANDLER_H__
