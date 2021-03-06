//////////////////////////////////////////////////////////////////////////////
// Filename    : AuraBall.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_AURA_BALL_HANDLER_H__
#define __SKILL_AURA_BALL_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class AuraBall;
//////////////////////////////////////////////////////////////////////////////

class AuraBall : public SkillHandler 
{
public:
	AuraBall()  {}
	~AuraBall()  {}
	
public:
    string getSkillHandlerName()  { return "AuraBall"; }

	SkillType_t getSkillType()  { return SKILL_AURA_BALL; }

	void execute(Slayer* pSlayer, ObjectID_t ObjectID, SkillSlot* pSkillSlot, CEffectID_t CEffectID);
	void execute(Monster* pMonster, Creature* pEnemy);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern AuraBall g_AuraBall;

#endif // __SKILL_AURA_BALL_HANDLER_H__
