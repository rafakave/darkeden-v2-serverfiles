//////////////////////////////////////////////////////////////////////////////
// Filename    : CreateHolyWater.h 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_CREATE_HOLY_WATER_HANDLER_H__
#define __SKILL_CREATE_HOLY_WATER_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class CreateHolyWater;
//////////////////////////////////////////////////////////////////////////////

class CreateHolyWater : public SkillHandler 
{
public:
	CreateHolyWater()  {}
	~CreateHolyWater()  {}
	
public:
    string getSkillHandlerName()  { return "CreateHolyWater"; }
	SkillType_t getSkillType()  { return SKILL_CREATE_HOLY_WATER; }

	void execute(Slayer* pSlayer, ObjectID_t, CoordInven_t X, CoordInven_t Y, CoordInven_t TargetX, CoordInven_t TargetY, SkillSlot* pSkillSlot);

	void computeOutput(const SkillInput& input, SkillOutput& output);

protected:
	bool canMake(ItemType_t waterType, int DomainLevel, int SkillLevel) ;
};

// global variable declaration
extern CreateHolyWater g_CreateHolyWater;

#endif // __SKILL_CREATE_HOLY_WATER_HANDLER_H__
