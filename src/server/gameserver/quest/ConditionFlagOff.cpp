////////////////////////////////////////////////////////////////////////////////
// Filename    : ConditionFlagOff.cpp
// Written By  : 
// Description :
////////////////////////////////////////////////////////////////////////////////

#include "ConditionFlagOff.h"
#include "PlayerCreature.h"
#include "FlagSet.h"

////////////////////////////////////////////////////////////////////////////////
// is satisfied?
////////////////////////////////////////////////////////////////////////////////
bool ConditionFlagOff::isSatisfied (Creature * pCreature1 , Creature * pCreature2, void* pParam) const 
	 
{ 
	Assert(pCreature2 != NULL);
	Assert(pCreature2->isPC());

	PlayerCreature* pPC = dynamic_cast<PlayerCreature*>(pCreature2);
	FlagSet* pFlagSet = pPC->getFlagSet();

	if (pFlagSet->isOn(m_Index) == false) return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void ConditionFlagOff::read (PropertyBuffer & propertyBuffer) 
	
{
	try
	{
		// read turn
		m_Index = propertyBuffer.getPropertyInt("Index");
	}
	catch (NoSuchElementException & nsee)
	{
		throw Error(nsee.toString());
	}
}

////////////////////////////////////////////////////////////////////////////////
	// get debug string
////////////////////////////////////////////////////////////////////////////////
string ConditionFlagOff::toString () const 
	 
{ 
	__BEGIN_TRY

	StringStream msg;
	msg << "ConditionFlagOff("
		<< "Index:" << (int)m_Index
		<< ")"; 
	return msg.toString();

	__END_CATCH
}
