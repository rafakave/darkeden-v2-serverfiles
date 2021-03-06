//////////////////////////////////////////////////////////////////////////////
// Filename    : EffectFlare.h
// Written by  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __EFFECT_FLARE__
#define __EFFECT_FLARE__

#include "Effect.h"
#include "EffectLoader.h"

//////////////////////////////////////////////////////////////////////////////
// class EffectFlare
//////////////////////////////////////////////////////////////////////////////

class EffectFlare : public Effect 
{
public:
	EffectFlare(Creature* pCreature);

public:
    EffectClass getEffectClass()  { return EFFECT_CLASS_FLARE; }

	void affect() {}
	void affect(Creature* pCreature);
	void affect(Zone* pZone, ZoneCoord_t x, ZoneCoord_t y, Object* pObject);

	void unaffect();
	void unaffect(Creature* pCreature);
	void unaffect(Item*  pItem)  {}
	void unaffect(Zone* pZone, ZoneCoord_t x, ZoneCoord_t y, Object* pObject);

	virtual void create(const string & ownerID);
	virtual void destroy(const string & ownerID);
	virtual void save(const string & ownerID);

	string toString() ;
	
public:

	int getLevel()  { return m_Level; }
	void setLevel(int Level)  { m_Level = Level; }

	Sight_t getOldSight()  { return m_OldSight; }
	void setOldSight(Sight_t OldSight)  { m_OldSight = OldSight; }

private:
	int		m_Level;
	Sight_t m_OldSight;

};

//////////////////////////////////////////////////////////////////////////////
// class EffectFlareLoader
//////////////////////////////////////////////////////////////////////////////

class EffectFlareLoader : public EffectLoader 
{
public:
	virtual Effect::EffectClass getEffectClass()  { return Effect::EFFECT_CLASS_FLARE; }
	virtual string getEffectClassName()  { return "EffectFlare"; }

public:
	virtual void load(Creature* pCreature);

};

extern EffectFlareLoader* g_pEffectFlareLoader;

#endif // __EFFECT_YELLOW_POISON_TO_CREATURE__
