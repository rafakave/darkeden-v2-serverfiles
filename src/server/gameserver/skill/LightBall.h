//////////////////////////////////////////////////////////////////////////////
// Filename    : LightBall.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_LIGHT_BALL_HANDLER_H__
#define __SKILL_LIGHT_BALL_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class LightBall;
//////////////////////////////////////////////////////////////////////////////

class LightBall : public SkillHandler 
{
public:
	LightBall()  {}
	~LightBall()  {}
	
public:
    string getSkillHandlerName()  { return "LightBall"; }

	SkillType_t getSkillType()  { return SKILL_LIGHT_BALL; }

	void execute(Slayer* pSlayer, ObjectID_t ObjectID, SkillSlot* pSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern LightBall g_LightBall;

#endif // __SKILL_LIGHT_BALL_HANDLER_H__
