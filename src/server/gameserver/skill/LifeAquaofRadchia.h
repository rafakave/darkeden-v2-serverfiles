//////////////////////////////////////////////////////////////////////////////
// Filename    : IceHorizon.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_LIFE_AQUA_OF_RADCHIA_HANDLER_H__
#define __SKILL_LIFE_AQUA_OF_RADCHIA_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class IceHorizon;
//////////////////////////////////////////////////////////////////////////////

class LifeAquaofRadchia : public SkillHandler 
{
public:
	LifeAquaofRadchia() ;
	~LifeAquaofRadchia()  {}
	
public:
    string getSkillHandlerName()  { return "LifeAquaofRadchia"; }
	SkillType_t getSkillType()  { return SKILL_LIFE_AQUA_OF_RADCHIA; }

	void execute(Ousters* pOusters, ZoneCoord_t X, ZoneCoord_t Y, OustersSkillSlot* pOustersSkillSlot, CEffectID_t CEffectID);
	void execute(Ousters* pOusters, ObjectID_t, OustersSkillSlot* pOustersSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);

};

// global variable declaration
extern LifeAquaofRadchia g_LifeAquaofRadchia;

#endif // __SKILL_ICE_HORIZON_HANDLER_H__
