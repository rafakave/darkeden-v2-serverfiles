//////////////////////////////////////////////////////////////////////////////
// Filename    : PlayingWithFire.h 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_PLAYING_WITH_FIRE_HANDLER_H__
#define __SKILL_PLAYING_WITH_FIRE_HANDLER_H__

#include "SkillHandler.h"

//////////////////////////////////////////////////////////////////////////////
// class PlayingWithFire;
//////////////////////////////////////////////////////////////////////////////

class PlayingWithFire: public SkillHandler 
{
public:
	PlayingWithFire() ;
	~PlayingWithFire()  {}
	
public:
    string getSkillHandlerName()  { return "PlayingWithFire"; }
	SkillType_t getSkillType()  { return SKILL_PLAYING_WITH_FIRE; }

	//void execute(Slayer* pSlayer, SlayerSkillSlot* pSlayerSkillSlot, CEffectID_t CEffectID);
	//void execute(Vampire* pVampire, VampireSkillSlot* pVampireSkillSlot, CEffectID_t CEffectID);
	void execute(Monster* pMonster);

	void computeOutput(const SkillInput& input, SkillOutput& output);

private :
	POINT	m_pPlayingWithFireMask[8];
};

// global variable declaration
extern PlayingWithFire g_PlayingWithFire;

#endif // __SKILL_HIDE_HANDLER_H__
