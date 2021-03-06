//////////////////////////////////////////////////////////////////////////////
// Filename    : WideIceHail.h 
// Written By  : 
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_WIDE_ICE_HAIL_HANDLER_H__
#define __SKILL_WIDE_ICE_HAIL_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class WideIceHail;
//////////////////////////////////////////////////////////////////////////////

class WideIceHail : public SkillHandler 
{
public:
	WideIceHail()  { }
	~WideIceHail()  {}
	
public:
    string getSkillHandlerName()  { return "WideIceHail"; }
	SkillType_t getSkillType()  { return SKILL_WIDE_ICE_HAIL; }

	void execute(Monster*, ZoneCoord_t X, ZoneCoord_t Y);

	void computeOutput(const SkillInput& input, SkillOutput& output);
};

// global variable declaration
extern WideIceHail g_WideIceHail;

#endif // __SKILL_WIDE_ICE_HAIL_HANDLER_H__
