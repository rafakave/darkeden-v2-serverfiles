//////////////////////////////////////////////////////////////////////////////
// Filename    : ActionGiveFinalLottoScript.h
// Written By  : excel96
// Description : 
// Creature가 PC에게 질문을 던진다. 이 대사는 NPC 대화창에 출력된다.
//////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_GIVE_FINAL_LOTTO_SCRIPT_H__
#define __ACTION_GIVE_FINAL_LOTTO_SCRIPT_H__

#include "Types.h"
#include "Exception.h"
#include "Action.h"
#include "ActionFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class ActionGiveFinalLottoScript
//////////////////////////////////////////////////////////////////////////////

class ActionGiveFinalLottoScript : public Action 
{
public:
	virtual ActionType_t getActionType()  { return ACTION_GIVE_FINAL_LOTTO_SCRIPT; }
	virtual void read(PropertyBuffer & propertyBuffer);
	virtual void execute(Creature* pCreature1, Creature* pCreature2 = NULL);
	virtual string toString() ;

public:

private:
	ScriptID_t	m_SuccessScriptID;
	ScriptID_t	m_FailScriptID;
	ScriptID_t	m_CounterScriptID;
};


//////////////////////////////////////////////////////////////////////////////
// class ActionGiveFinalLottoScriptFactory;
//////////////////////////////////////////////////////////////////////////////

class ActionGiveFinalLottoScriptFactory : public ActionFactory 
{
public:
	virtual ActionType_t getActionType()  { return Action::ACTION_GIVE_FINAL_LOTTO_SCRIPT; }
	virtual string getActionName()  { return "GiveFinalLottoScript"; }
	virtual Action* createAction()  { return new ActionGiveFinalLottoScript(); }
};
#endif
