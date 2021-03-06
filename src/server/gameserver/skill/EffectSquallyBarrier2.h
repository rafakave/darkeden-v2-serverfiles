//////////////////////////////////////////////////////////////////////////////
// Filename    : EffectConcealment.h
// Written by  : crazydog
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __EFFECT_SQUALLY_BARRIER2__
#define __EFFECT_SQUALLY_BARRIER2__

#include "Effect.h"
#include "EffectLoader.h"

//////////////////////////////////////////////////////////////////////////////
// class EffectConcealment
//////////////////////////////////////////////////////////////////////////////

class EffectSquallyBarrier2 : public Effect 
{
public:
	EffectSquallyBarrier2(Creature* pCreature);

public:
    EffectClass getEffectClass()  { return EFFECT_CLASS_SQUALLY_BARRIER2; }

	void affect() {}

	void unaffect(Creature* pCreature);
	void unaffect();

	string toString() ;

public:
	Level_t getLevel()  { return m_Level; }
	void setLevel(Level_t Level) ;

	EffectClass getClientEffectClass()  { return m_ClientEffectClass; }
	void setClientEffectClass(EffectClass effectClass)  { m_ClientEffectClass = effectClass; }

private:
	Level_t 		m_Level;				// 스킬 레벨
	EffectClass 	m_ClientEffectClass;	// 클라이언트에 보내줄때 쓰는 이펙트 클래스 아이디

};

//////////////////////////////////////////////////////////////////////////////
// class EffectSquallyBarrier2Loader
//////////////////////////////////////////////////////////////////////////////

class EffectSquallyBarrier2Loader : public EffectLoader 
{
public:
	virtual Effect::EffectClass getEffectClass()  { return Effect::EFFECT_CLASS_SQUALLY_BARRIER2; }
	virtual string getEffectClassName()  { return "EffectSquallyBarrier2"; }

public:
	virtual void load(Creature* pCreature)  {}
};


#endif // __EFFECT_CONCEALMENT__
