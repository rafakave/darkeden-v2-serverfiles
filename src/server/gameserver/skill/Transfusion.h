//////////////////////////////////////////////////////////////////////////////
// Filename    : Transfusion.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_TRANSFUSION_HANDLER_H__
#define __SKILL_TRANSFUSION_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class Transfusion;
//////////////////////////////////////////////////////////////////////////////

class Transfusion : public SkillHandler 
{
public:
	Transfusion()  {}
	~Transfusion()  {}

public:
    string getSkillHandlerName()  { return "Transfusion"; }
	SkillType_t getSkillType()  { return SKILL_TRANSFUSION; }

	void execute(Vampire* pVampire, ZoneCoord_t X, ZoneCoord_t Y,  VampireSkillSlot* pVampireSkillSlot, CEffectID_t CEffectID);
	void execute(Vampire* pVampire, ObjectID_t,  VampireSkillSlot* pVampireSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern Transfusion g_Transfusion;

#endif // __SKILL_TRANSFUSION_HANDLER_H__
