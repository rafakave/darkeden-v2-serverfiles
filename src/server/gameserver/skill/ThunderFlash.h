//////////////////////////////////////////////////////////////////////////////
// Filename    : ThunderFlash.h 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_THUNDER_FLASH_HANDLER_H__
#define __SKILL_THUNDER_FLASH_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class ThunderFlash;
//////////////////////////////////////////////////////////////////////////////

class ThunderFlash : public SkillHandler 
{
public:
	ThunderFlash() ;
	~ThunderFlash()  {}
	
public:
    string getSkillHandlerName()  { return "ThunderFlash"; }
	SkillType_t getSkillType()  { return SKILL_THUNDER_FLASH; }

	void execute(Slayer* pSlayer, ObjectID_t ObjectID,  SkillSlot* pSkillSlot, CEffectID_t CEffectID);
	void execute(Slayer* pSlayer, ZoneCoord_t X, ZoneCoord_t Y, SkillSlot* pSkillSlot, CEffectID_t CEffectID);

	void computeOutput(const SkillInput& input, SkillOutput& output);

protected:
	POINT m_pThunderFlashMask[4];

};

// global variable declaration
extern ThunderFlash g_ThunderFlash;

#endif // __SKILL_THUNDER_FLASH_HANDLER_H__
