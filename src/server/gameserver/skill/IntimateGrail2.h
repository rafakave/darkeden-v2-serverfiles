//////////////////////////////////////////////////////////////////////////////
// Filename    : IntimateGrail.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_INTIMATE_GRAIL2_HANDLER_H__
#define __SKILL_INTIMATE_GRAIL2_HANDLER_H__

#include "SkillHandler.h"
#include "Creature.h"
//////////////////////////////////////////////////////////////////////////////
// class IntimateGrail;
//////////////////////////////////////////////////////////////////////////////

class IntimateGrail2 : public SkillHandler 
{
public:
	IntimateGrail2()  {}
	~IntimateGrail2()  {}
	
public :
    string getSkillHandlerName()  { return "IntimateGrail2"; }
	SkillType_t getSkillType()  { return SKILL_INTIMATE_GRAIL2; }

	void execute(Slayer* pSlayer, ObjectID_t ObjectID,  SkillSlot* pSkillSlot, CEffectID_t CEffectID);
	void execute(Slayer* pSlayer, SkillSlot* pSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern IntimateGrail2 g_IntimateGrail2;

#endif // __SKILL_INTIMATE_GRAIL_HANDLER_H__
