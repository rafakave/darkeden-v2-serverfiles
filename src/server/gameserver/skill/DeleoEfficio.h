//////////////////////////////////////////////////////////////////////////////
// Filename    : DeleoEfficio.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_DELEO_EFFICIO_HANDLER_H__
#define __SKILL_DELEO_EFFICIO_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class DeleoEfficio;
//////////////////////////////////////////////////////////////////////////////

class DeleoEfficio : public SkillHandler 
{
public:
	DeleoEfficio()  {}
	~DeleoEfficio()  {}
	
public:
    string getSkillHandlerName()  { return "DeleoEfficio"; }
	SkillType_t getSkillType()  { return SKILL_DELEO_EFFICIO; }

	void execute(Ousters* pOusters, ObjectID_t ObjectID, OustersSkillSlot* pSkillSlot, CEffectID_t CEffectID);
	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern DeleoEfficio g_DeleoEfficio;

#endif // __SKILL_DELEO_EFFICIO_HANDLER_H__
