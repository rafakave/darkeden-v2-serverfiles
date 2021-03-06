//////////////////////////////////////////////////////////////////////////////
// Filename    : Paralyze.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_CURSE_OF_BLOOD1_HANDLER_H__
#define __SKILL_CURSE_OF_BLOOD1_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class Paralyze;
//////////////////////////////////////////////////////////////////////////////

class CurseOfBlood1: public SkillHandler 
{
public:
	CurseOfBlood1()  {}
	~CurseOfBlood1()  {}
	
public:
    string getSkillHandlerName()  { return "CurseOfBlood1"; }
	SkillType_t getSkillType()  { return SKILL_CURSE_OF_BLOOD1; }

	void execute(Vampire* pSlayer, VampireSkillSlot* pSkillSlot, CEffectID_t CEffectID);
	//void execute(Monster* pMonster, Creature* pEnemy);

	//void executeMonster(Zone* pZone, Monster* pMonster, Creature* pEnemy);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern CurseOfBlood1 g_CurseOfBlood1;

#endif // __SKILL_PARALYZE_HANDLER_H__
