//////////////////////////////////////////////////////////////////////////////
// Filename    : AcidBall.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_ACID_BALL_HANDLER_H__
#define __SKILL_ACID_BALL_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class AcidBall
//////////////////////////////////////////////////////////////////////////////

class AcidBall : public SkillHandler 
{
public:
	AcidBall()  {}
	~AcidBall()  {}
	
public:
    string getSkillHandlerName()  { return "AcidBall"; }
	SkillType_t getSkillType()  { return SKILL_ACID_BALL; }

	void execute(Vampire* pVampire, ObjectID_t ObjectID,  VampireSkillSlot* pVampireSkillSlot, CEffectID_t CEffectID);
	void execute(Monster* pMonster, Creature* pEnemy);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern AcidBall g_AcidBall;

#endif // __SKILL_ACID_BALL_HANDLER_H__
